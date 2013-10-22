package HgWeb::Controller::Currency;
use Mojo::Base 'HgWeb::Controller';
use Hg::Currency;

sub list {
    my $self = shift;

    Hg::Account->config(-DBH => $self->dbh);
    $self->render(template => 'app/account');
}


sub create {
    my $self = shift;

    my $acc_name = $self->param('acc_name') || return $self->render_exception('unknown account name');
    my $acc_currency = $self->param('acc_currency') || return $self->render_exception('unknown currency');

    Hg::Account->config(-DBH => $self->hg);
    
    $self->stash(acc_name  => $acc_name);
    $self->stash(acc_currency  => $acc_currency);
    
    $self->render(template => 'app/account');
}

1;
