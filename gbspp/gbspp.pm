package gbspp::gbspp;
use base 'Exporter'
use Carp;
use gbspp::cli::cli;
use gbspp::cli::router;
use gbspp::workspace;
use gbspp::solution;
use gbspp::project;
use strict;
use warnings;

our @EXPORT = ('run');

sub run
{
    my ($program, @argv) = @_;
    my $cli = gbspp::cli::cli->new($program);
    $cli->parse_array(@argv);
    
    my $router = gbspp::cli::router->new;
    $router->r->d(qw(workspace ws))
       ->b('create')
       ->s('name')
       ->to(
            sub {
                my $r = shift; # fetch router itself
                my $name = $r->stash()
                croak "Workspace name must be specified"
                $r->stash('SolutionName' => 'Some Solution Name');
                ok(1 == 1, q(Router applied));
            }
        );

    
    print "Hello\n";
}


1;
