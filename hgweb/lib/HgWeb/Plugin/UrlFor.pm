package HgWeb::Plugin::UrlFor;
use Mojo::Base 'Mojolicious::Plugin';
use Mojo::ByteStream 'b'; # for prevent escaping

our $VERSION = 0.1;

sub register {
    my ($plugin, $app, $conf) = @_;
    
    # Initialize
    my $namespace = $conf->{namespace} || ( (ref $app) . '::UrlFor' );

    $app->helper(url_for_style => sub {
        my $self = shift;
        my $css = '<link rel="stylesheet" href="'
            . $self->url_for(_build_path('css', @_))
            . '">';
        # prevent escaping
        b($css);
    });

    $app->helper(url_for_javascript => sub {
        my $self = shift;
        my $js = '<script type="text/javascript" src="'
            . $self->url_for(_build_path('js', @_))
            . '"></script>';
        # prevent escaping
        b($js);
    });
}

sub _build_path
{
    my ($ext, $dir, $version, $name) = @_;

    unless (defined $name) {
        $name = $version;
        undef $version;
    }
    
    die 'Style file name expected (without extension)' unless $name;
    return $dir
        . ($dir ? '/' : '')
        . ($version ? $version . '/' : '')
        . $name
        . '.' . $ext;
}

1;
