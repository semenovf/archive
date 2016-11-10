#!/usr/bin/perl -w
use lib qw(../../);
use Test::Simple tests => 35;
use strict;

package test0;
use Test::Simple;
use gbspp::cli::cli;
#use Data::Dumper;
use strict;

sub run
{
    my @cmdline = (
          'domain'
        , '--bool-opt'
        , '--int-opt=123'
        , 'arg0'
        , '--char-opt=a'
        , '--string-opt="string option"'
        , 'arg1'
        , 'arg2'
    );
    
    my $cli = gbspp::cli::cli->new($0);
    
    ok($cli->parse_array(@cmdline), q(parse command line));
    
#    print Dumper($cli), "\n";
    
    ok($cli->has_domain                 , q(has domain));
    ok($cli->domain eq 'domain'         , q(domain 'domain' found));
    ok($cli->has_option('bool-opt')     , q('bool-opt' found));
    ok($cli->has_option('int-opt')      , q('int-opt' found));
    ok($cli->has_option('char-opt')     , q('char-opt' found));
    ok($cli->has_option('string-opt')   , q('short-opt' found));
    ok(! $cli->has_option('unknown-opt'), q('unknown-opt' not found));
    
    ok($cli->arg(0) eq 'arg0' , q(arg[0] eq 'arg0'));
    ok($cli->arg(1) eq 'arg1' , q(arg[1] eq 'arg1'));
    ok($cli->arg(2) eq 'arg2' , q(arg[2] eq 'arg2'));
    ok(! defined $cli->arg(3) , q(index '3' is out of bounds));
    ok(! defined $cli->arg(-1), q(index '0' is out of bounds));
    
    ok((defined $cli->optval('bool-opt') and $cli->optval('bool-opt') == 1), q('bool-opt' is true));
    ok((defined $cli->optval('bool-opt') and $cli->optval('bool-opt') != 0), q('bool-opt' is not false));
    ok((defined $cli->optval('int-opt') and $cli->optval('int-opt') == 123), q('int-opt' is equal to 123));
    ok((defined $cli->optval('int-opt') and $cli->optval('int-opt') != 124), q('int-opt' is not equal to 123));
    ok((defined $cli->optval('char-opt') and $cli->optval('char-opt') eq 'a'), q('char-opt' is equal to 'a'));
    ok((defined $cli->optval('char-opt') and $cli->optval('char-opt') ne 'b'), q('char-opt' is not equal to 'b'));
    ok((defined $cli->optval('string-opt') and $cli->optval('string-opt') eq 'string option'), q('string-opt' is equal to 'string option'));
    ok((defined $cli->optval('string-opt') and $cli->optval('string-opt') ne 'any option'), q('string-opt' is not equal to 'any option'));
}

package test1;
use Test::Simple;
use gbspp::cli::cli;
use strict;

sub run
{
    my @cmdline = (
          '--int-opt=123'
        , '--string-opt="string option0"'
        , '--int-opt=124'
        , '--string-opt="string option1"'
        , '--int-opt=125'
        , '--string-opt="string option2"'
        , '--float-opt=3.14159'
    );
    
    my $cli = gbspp::cli::cli->new($0);
    
    ok($cli->parse_array(@cmdline), q(parse command line));
    
    ok(! $cli->has_domain       , q(has no domain));
    ok($cli->nargs == 0         , q(no arguments exist));
    ok($cli->has_option('int-opt')      , q('int-opt' found));
    ok($cli->has_option('string-opt')   , q('short-opt' found));
    ok($cli->has_option('float-opt')    , q('float-opt' found));
    ok(! $cli->has_option('unknown-opt'), q('unknown-opt' not found));
    ok((defined $cli->optval('int-opt') and $cli->optval('int-opt') == 125), q(last value for 'int-opt' is 125));
    ok((defined $cli->optval('string-opt') and $cli->optval('string-opt') eq 'string option2'), q(last value for 'string-opt' is 'string option2'));
    ok((defined $cli->optval('float-opt') and $cli->optval('float-opt') eq '3.14159'), q(last value for 'float-opt' is '3.14159'));
    
    my @int_opt_values    = $cli->optval('int-opt');
    my @string_opt_values = $cli->optval('string-opt');
    my @float_opt_values  = $cli->optval('float-opt');
    
    ok((scalar(@int_opt_values)    == 3
            and $int_opt_values[0] + $int_opt_values[1] + $int_opt_values[2] == 123 + 124 + 125)
        , q('int-opt' has three values));
    
    ok((scalar(@string_opt_values) == 3
            and join('', @string_opt_values) eq 'string option0string option1string option2')
        , q('string_opt_values' has three values));
    
    ok((scalar(@float_opt_values)  == 1 and $float_opt_values[0] + 0.0 == 0.0 + '3.14159')
        , q('float-opt' has one value));
}

package test2;
use Test::Simple;
use gbspp::cli::cli;
use gbspp::cli::router;
use strict;

sub run
{
    my @cmdline = qw(
        project
        --create
        --name="SomeProjectName"
        --type="shared-lib"
        --depends=first_dep
        --depends=second_dep
        --depends=third_dep
        --enable-qt
        --doxygen
    );

    my $cli = gbspp::cli::cli->new($0);

    ok($cli->parse_array(@cmdline), q(parse command line));

    my $router = gbspp::cli::router->new;
    $router->r->d(qw(project pro prj))
       ->b('create')
#        ->s('name'     , 'ProjectName')
#        ->s('type'     , 'ProjectType' => {-default => 'console-app', -variants => qw(console-app gui-app shared-lib static-lib) })
#        ->s('lang'     , 'ProjectLang' => 'C++')
#        ->s('depends'  , 'ProjectDeps' => [])
#        ->b('enable-qt', 'EnableQt'    => 0)
#        ->b('doxygen'  , 'GenDoxyfile' => 0)
        ->to(
            sub {
                my $r = shift; # fetch router itself
                $r->stash('SolutionName' => 'Some Solution Name');
                ok(1 == 1, q(Router applied));
            }
        );

#    $router->run($cli);
}

package main;
test0::run;
test1::run;
test2::run;

0;


