package HgWeb;
use Mojo::Base 'Mojolicious';
use Mojolicious::Plugin::Database;

#BEGIN { $ENV{MOJO_I18N_DEBUG} = 1 };
BEGIN { $ENV{MOJO_LOG_LEVEL} = 'debug' };

# This method will run once at server start
sub startup {
    my $self = shift;
    
    # Documentation browser under "/perldoc"
    #$self->plugin('PODRenderer');
    
    # Internationalization
    $self->plugin('I18N', default => 'en_US' );
    
    # Require multiple databases at the same time
    $self->plugin('database', { 
    databases => {
        'dbh' => { 
            dsn      => 'dbi:mysql:dbname=hg'
          , username => 'hg'
          , password => '1qazxsw2'
          , host     => 'localhost'
          , port     => 3306
        }
    }
    });
    
    # Session management
    $self->secret('GvTrd2tFxUEKC2rb');
    
    # Router
    my $r = $self->routes;
    
    # Normal route to controller
    $r->any('/account')          ->to('account#list');
    $r->any('/account/create')   ->to('account#create');
    $r->any('/(*)')              ->to(cb => sub {
        my $self = shift;
#        $self->res->code(404);
#        $self->res->message('Not Found');
#        return $self->render_json({ ErrorCode => 404, Message => 'URI Not Found' });
#        return $self->render({ status => 404, text => 'Not Found' });
        return $self->render_not_found();
    });
}

1;
