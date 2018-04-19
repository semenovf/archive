#!/usr/bin/perl -w

use JSON;
use Data::Dumper;

my $methodCall = {
    'method'=>'methodName',
    'args' => {
        'arg0' => 'argVal0',
        'arg1' => 'argVal1',
        'arg2' => 'argVal2'
    }
};
my $json_text = to_json($methodCall);
my $perl_scalar = from_json( $json_text );
print $json_text, "\n\n";
print Dumper $perl_scalar, "\n";

0;
