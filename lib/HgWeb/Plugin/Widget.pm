package HgWeb::Plugin::Widget;
use Mojo::Base 'Mojolicious::Plugin';
use Mojo::ByteStream 'b'; # for prevent escaping

use constant { TRUE => 1, FALSE => 0 };

our $VERSION = 0.1;

sub register
{
    my ($plugin, $app, $conf) = @_;
    
    $plugin->{__hg_widgets__} = {};
    $plugin->{__hg_classes__} = {};
    
    # Initialize
    my $namespace = $conf->{namespace} || ( (ref $app) . '::Widget' );

    $app->helper(w => sub {
        my ($self, $id, $class) = @_;
        my $widgets = $plugin->{__hg_widgets__} or die;
        
        unless (defined $id) {
            $class = "HgWidget::Widget";
            _require_class($class);
            _require_methods($class, 'global');
            return $class->global;
        }
        
        if (defined $class) { # is widget's instance definition
            die qq(Widget with id '$id' already exists) if defined $widgets->{$id};
            $class = "HgWidget::$class";
            
            _require_class($class);
            _require_methods($class, 'new');
            
            $widgets->{$id} = $class->new(id => $id);
            
            if(defined $plugin->{__hg_classes__}->{$class}) {
                ++ $plugin->{__hg_classes__}->{$class};
            } else {
                $plugin->{__hg_classes__}->{$class} = 1;
            }
        }
        
        die qq(Widget with id '$id' not found) unless defined $widgets->{$id};
        $widgets->{$id};
    });

    $app->helper(widget_controllers => sub {
        my $self = shift;
        my $widgets = $plugin->{__hg_widgets__} // {};
        
        # Traverse through widgets to extract used classes.
        my $js = '';
        while (my ($id, $w) = each(%{$widgets})) {
            my $c = ref $w;
            
            if ($c->can('controller')) {
                $js .= $w->controller(@_);
            }
        }
        b($js);
    });
                 
    $app->helper(widget_imports => sub {
        my $self = shift;
        my @import_styles = ();
        my @import_scripts = ();
        my $classes = $plugin->{__hg_classes__} // {};
        my @classes = ('HgWidget::Widget', keys(%{$classes}));
        
        foreach my $c (@classes) {
            _require_class($c);

            if ($c->can('_import_styles')) {
                push @import_styles, @{$c->_import_styles};
            }

            if ($c->can('_import_scripts')) {
                push @import_scripts, @{$c->_import_scripts};
            }
        }

        my %imported_styles = ();
        my %imported_scripts = ();
        my $imports = '';
        
        foreach my $style (@import_styles) {
            next if defined $imported_styles{$style};
            $imported_styles{$style} = 1;
            
            $imports .= "\n" if length $imports > 0;
            $imports .= '<link rel="stylesheet" href="'
            . $self->url_for($style)
            . '">';
        }

        foreach my $script (@import_scripts) {
            next if defined $imported_scripts{$script};
            $imported_scripts{$script} = 1;

            $imports .= "\n" if length $imports > 0;
            $imports .= '<script type="text/javascript" src="'
            . $self->url_for($script)
            . '"></script>';
        }
        
        b($imports);
    });

    $app->helper(widget_view => sub {
        my $self = shift;
        my $id = shift or die 'Widget\'s identifier is mandatory';
        my $widgets = $plugin->{__hg_widgets__} or die;
        
        die qq(Widget with id '$id' does not exist) unless defined $widgets->{$id};
        # prevent escaping
        b($widgets->{$id}->view(@_));
    });
}

sub _cache_classes
{
    
}

sub _require_class
{
    my $class = shift or die;

    (my $file = $class) =~ s/::|'/\//g;
    eval {require "$file.pm"};

    die qq[Unable to load of $class: $@] if $@;
}

sub _require_methods
{
    my $class = shift or die;
    my @methods = @_;
    my $ok = TRUE;
    
    foreach my $m (@methods) {
        unless ($class->can($m)) {
            warn qq/Can't locate method '$m' via package HgWidget::Config/;
            $ok = FALSE;
        }
    }
    die unless $ok;
}

1;
