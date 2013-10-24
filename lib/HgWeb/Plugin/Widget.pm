package HgWeb::Plugin::Widget;
use Mojo::Base 'Mojolicious::Plugin';
use Mojo::ByteStream 'b'; # for prevent escaping

use constant { TRUE => 1, FALSE => 0 };

our $VERSION = 0.1;

my %_widgets = ();

sub register
{
    my ($plugin, $app, $conf) = @_;
    
    # clear widgets
    %_widgets = ();
    
    # Initialize
    my $namespace = $conf->{namespace} || ( (ref $app) . '::Widget' );

    $app->helper(w => sub {
        my $self = shift;
        my $id = shift or die 'Widget\'s identifier is mandatory';
        my $class = shift;
        
        if (defined $class) { # is widget's instance definition
            die qq(Widget with id '$id' already exists) if defined $_widgets{$id};
            $class = "HgWidget::$class";
            die unless _import_and_check($class, 'new');
            $_widgets{$id} = $class->new(id=>$id);
        } else {
            die qq(Widget with id '$id' not found) unless defined $_widgets{$id};
            return $_widgets{$id};
        }
    });

#    $app->helper(wb => sub {
#        my $self = shift;
#        my $html = shift or '';
#        b($html);
#    });
    
    $app->helper(widget_config => sub {
        my $self = shift;
        my %conf = @_;

        my $class = "HgWidget::Config";
        die unless _import_and_check($class, 'update');
        while (my ($key, $value) = each(%conf)) {
            $class->update($key, $value);
        }
    });

    $app->helper(widget_import => sub {
        my $self = shift;
        my @classes = @_;
        
        my $class = "HgWidget::Config";
        die unless _import_and_check($class, 'import_styles', 'import_scripts');
        my @import_styles = $class->import_styles(@classes);
        my @import_scripts = $class->import_scripts(@classes);
        my $imports = '';
        
        foreach my $style (@import_styles) {
            $imports .= "\n" if length $imports > 0;
            $imports .= '<link rel="stylesheet" href="'
            . $self->url_for($style)
            . '">';
        }

        foreach my $script (@import_scripts) {
            $imports .= "\n" if length $imports > 0;
            $imports .= '<script type="text/javascript" src="'
            . $self->url_for($script)
            . '"></script>';
        }
        
        return b($imports);
    });

    $app->helper(widget_controller => sub {
        my $self = shift;
        my $id = shift or die 'Widget\'s identifier is mandatory';
        die qq(Widget with id '$id' does not exist) unless defined $_widgets{$id};
        my $js = '<script type="text/javascript">' . "\n"
            . $_widgets{$id}->controller(@_)
            . '</script>';
        # prevent escaping
        b($js);
    });

    $app->helper(widget_view => sub {
        my $self = shift;
        my $id = shift or die 'Widget\'s identifier is mandatory';
        die qq(Widget with id '$id' does not exist) unless defined $_widgets{$id};
        # prevent escaping
        b($_widgets{$id}->view(@_));
    });
}


sub _widget_create
{
    my $class = shift or die 'Widget\'s class name is mandatory';
    my $id = shift or die 'Widget\'s identifier is mandatory';
    
    die qq(Widget with id '$id' already exists) if defined $_widgets{$id};
    
    $class = "HgWidget::$class";
    die unless _import_and_check($class, 'new');
    $_widgets{$id} = $class->new($id);
}

sub _import_and_check
{
    my $class = shift or die;
    my @functions = @_;
    my $ok = TRUE;
    
    (my $file = $class) =~ s/::|'/\//g;
    eval {require "$file.pm"};

    if ($@) {
        warn qq/Unable to load of $class: $@/;
        $ok = FALSE;
    } else {
        foreach my $f (@functions) {
            unless ($class->can($f)) {
                warn qq/Can't locate method '$f' via package HgWidget::Config/;
                $ok = FALSE;
            }
        }
    }
    return $ok;
}

1;
