package HgWeb::Controller::Websocket;
use Mojo::Base 'Mojolicious::Controller';
use Mojo::IOLoop; # for websocket

sub index
{
    my $self = shift;
    $self->render(template => 'app/websocket');
    
}

sub echo
{
    my $self = shift;

    # Opened
    $self->app->log->debug('WebSocket opened.');

    # Increase inactivity timeout for connection a bit
    Mojo::IOLoop->stream($self->tx->connection)->timeout(300);

    # Incoming message
    $self->on(message => sub {
        my ($self, $msg) = @_;
        $self->send("echo: $msg");
    });

    # Closed
    $self->on(finish => sub {
        my ($self, $code, $reason) = @_;
        $self->app->log->debug("WebSocket closed with status $code.");
    });
}

1;
