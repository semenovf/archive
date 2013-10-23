package HgWeb;
use Mojo::Base 'Mojolicious';
use Mojolicious::Plugin::Database;
use HgWeb::Plugin::UrlFor;

#BEGIN { $ENV{MOJO_I18N_DEBUG} = 1 };
BEGIN { $ENV{MOJO_LOG_LEVEL} = 'debug' };

# This method will run once at server start
sub startup {
    my $self = shift;
    
    # Add another namespace to load plugins from
    my $plugins = $self->plugins;
    push @{$plugins->namespaces}, 'HgWeb::Plugin';
    
    # Documentation browser under "/perldoc"
    #$self->plugin('PODRenderer');
    
    $self->plugin('UrlFor');
    $self->plugin('Widget');

    # Internationalization
    $self->plugin('I18N', default => 'en_US' );
    
    # Require multiple databases at the same time
    $self->plugin('database', { 
    databases => {
        'dbh' => { 
            dsn      => 'dbi:mysql:dbname=hg:host=localhost:port=3306'
          , username => 'hg'
          , password => '1qazxsw2'
          #, host     => 'localhost'
          #, port     => 3306
        }
    }
    });
    
    # Session management
    $self->secret('GvTrd2tFxUEKC2rb');
    
    # Router
    my $r = $self->routes;
    $r->namespaces(['HgWeb::Controller']);
    
    # Normal route to controller
    $r->get('/account')         ->to('account#list');
    $r->post('/account/create') ->to('account#create');
    
    $r->get('/tr')             ->to('transaction#list');
    #$r->post('/tr/create')     ->to('transaction#create');
    $r->get('/tr/create')         ->to('transaction#create');
    $r->get('/tr/create/expander')->to('transaction#expander');
   
    # WebSocket echo service
    $r->get('/ws')->to('websocket#index');
    $r->websocket('/ws/echo')->to('websocket#echo');
    
    $r->any('/(*)')            ->to(cb => sub {
        my $self = shift;
#        $self->res->code(404);
#        $self->res->message('Not Found');
#        return $self->render_json({ ErrorCode => 404, Message => 'URI Not Found' });
#        return $self->render({ status => 404, text => 'Not Found' });
        return $self->render_not_found();
    });
}

1;
