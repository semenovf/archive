package HgWeb::Controller::Transaction;
use Mojo::Base 'HgWeb::Controller';

sub list
{
    my $self = shift;
    $self->render_text('Transaction List');
}


sub create
{
    my $self = shift;
    $self->render(template => 'app/transaction');
}

sub expander
{
    my $self = shift;
    #$self->render(json => {header => 'Header', content => 'Content data'});
    $self->render(json => {content => 'Content data'});
}

1;
