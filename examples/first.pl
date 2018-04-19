#!/usr/bin/perl -w
use lib qw(../lib);
use DBI::DDI;
use strict;
use warnings;

my $ddi_auth = {
    'roles' => { # roles, e.g. Admin|Superuser, Owner|Author, Editor, Viewer, Guest|Anonymous etc.
        'id'                => DDI_PK(0, 100),
        'name'              => DDI_TEXT(64),
        'desc'              => DDI_TEXT
    },
    'perms' => { # permissions
        'id'                => DDI_PK(0, 100),
        'name'              => DDI_TEXT(64),
        'desc'              => DDI_TEXT
    },
    'roles_perms' => { #
        'id'                => DDI_PK(0, 100*100),
        'role_id'           => DDI_REF('jq_auth_roles'),
        'perl_id'           => DDI_REF('jq_auth_perms')
    },
    'users' => {
        'id'                => DDI_PK(0, DDI_MAX_INT),
        'role_id'           => DDI_REF('jq_auth_roles'),
        'login'             => DDI_TEXT(32),
        'first_name'        => DDI_TEXT(64),
        'last_name'         => DDI_TEXT(64)
    }
};

my $dbh;

my $ddi = DBI::DDI::Class->new;
$ddi->dbh($dbh);
$ddi->ddi($ddi_auth);
$ddi->impl('DBI::DDI::MySQL');
$ddi->ns('auth_db');
$ddi->charset('utf8');
$ddi->prepare(-Prefix=>'auth_', -Suffix=>'_t');
$ddi->deploy;
$ddi->recall;


prepare($ddi_auth, -Prefix=>'auth_', -Suffix=>'_t');
deploy(
    -DBH=>$dbh,
    -DDI=>$ddi_auth,
    -Impl=>'DBI::DDI::MySQL',
    -NS=>'auth_db',
    -Charset=>'utf8');

recall(
    -DBH=>$dbh,
    -DDI=>$ddi_auth,
    -Impl=>'DBI::DDI::MySQL',
    -NS=>'auth_db',
    -DropDB=>1);