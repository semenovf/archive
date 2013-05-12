package HgWeb::Controller::Transaction;
use Mojo::Base 'Mojolicious::Controller';

sub list {
    my $self = shift;
    $self->render_text('Transaction List');
}


sub create {
    my $self = shift;
    $self->render(template => 'app/transaction');
}

1;
