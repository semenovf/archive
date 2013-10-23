package HgWeb::Plugin::Widget;
use Mojo::Base 'Mojolicious::Plugin';
use Mojo::ByteStream 'b'; # for prevent escaping

our $VERSION = 0.1;

my %_widgets =();

sub register {
    my ($plugin, $app, $conf) = @_;
    
    # Initialize
    my $namespace = $conf->{namespace} || ( (ref $app) . '::Widget' );

    $app->helper(widget => sub {
        my $self = shift;
        my $class = shift or die 'Widget\'s class name is mandatory';
        my $id = shift or die 'Widget\'s identifier is mandatory';

        die qq(Widget with id '$id' already exists) if defined $_widgets{$id};
        
        #my $widget = eval {require HgWeb::Plugin::$class; HgWeb::Plugin::$class->new(id=>$id)};
        my $class = "HgWidget::$class";
        (my $file = $class) =~ s/::|'/\//g;
        eval {require "$file.pm"};
        
        die qq/Unable to create instance of HgWidget::$class: $@/ if $@;
        die qq/Constractor ('new') not found in HgWidget::$class/ unless $class->can('new');
        $_widgets{$id} = $class->new($id);
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

1;
