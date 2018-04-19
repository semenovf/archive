#!/usr/bin/perl -w

use JQ::RC;

my $rc0 = rc_fail('failed');
my $rc1 = rc_ok('is ok');
my $rc2 = rc_ok(234);



print q(isOK for rc_failed is 'false': ), $rc0->isOk ? 'true' : 'false', "\n";
print q(isOK for rc_ok is 'true': ), $rc1->isOk ? 'true' : 'false', "\n";

print q(Message from rc_fail must be 'failed': ), $rc0->errstr, "\n";
print q(Return code from rc_ok must be 'is ok': ), $rc1->rc, "\n";
print q(Return code from rc_ok must be '234': ), $rc2->rc, "\n";

0;
