#
# Copyright (C) 2010 Vladislav Trifochkin
# MetaPage::Parser is a part of Met@Page software
# Met@Page is free software; you can redistribute it and/or
# modify it under the same terms as Perl itself.
#
package MetaPage::Parser;
use base qw(XML::Parser::Expat Class::Accessor);
use strict;
use warnings;

__PACKAGE__->mk_accessors(qw(indent increment ignoreXMLUnexpectedEvent));


=head MetaPage::Parser->ignoreUnexpectedEvent(bool)
    If bool set to TRUE then ExUnexpectedEvent will not rise.
    FALSE by default.
=cut

sub new
{
    my $class = shift or die;
    my $self = $class->SUPER::new(@_);
    $self->indent(0);
    $self->increment(4);
    $self->ignoreXMLUnexpectedEvent(0);
    return $self;
}

sub incrementIndent
{
    my $self = shift or die;
    $self->indent( $self->indent + $self->increment );
    return $self;
}

sub decrementIndent
{
    my $self = shift or die;
    my $indent = $self->indent - $self->increment;
    $self->indent( $indent > 0 ? $indent : 0 );
    return $self;
}

sub elements
{
    my $self = shift or die;
    $self->{'elements'} = {} unless exists $self->{'elements'};
    return $self->{'elements'};
}

sub code
{
    my $self = shift or die;
    $self->{'code'} = [] unless exists $self->{'code'};
    return $self->{'code'};
}

sub addCode
{
    my $self = shift or die;
    my $code_ref = $self->code;
    push @{$code_ref}, @_;
    return $self;
}

sub renderCode
{
    my $self = shift or die;
    my $code_ref = $self->code;
    foreach ( @{$code_ref} ) {
        print $_, "\n";
    }
    return $self;
}

#
# MetaPage::parse( $fh_rule, $fh_tpl, $fh_out )
#
# @param $fh_rule   rules file's descriptor
# @param $fh_tpl    template file's descriptor
# @param $fh_out    output file's descriptor
#
#sub parse
#{
#    my ( $self, $fh_rules, $fh_tmpl, $fh_out ) = @_;
#
#    # тип - исполняемый скрипт или модуль
#    my $isExe = $self->opt('output.type') eq 's' ? 1 : 0;
#
#    $fh_out = *STDOUT unless $fh_out;
#
#    eval { # try
#        # parse rules
#        $self->parse_rules( $fh_rules );
#        # parse template
#        $self->parse_template($fh_tmpl);
#    }; if( $@ ) { # Catch
#        chomp $@;
#        $@ =~ tr/\n/ /;
#        die "ERROR: ", $@, "\n";
#        return 0;
#    }
#
#
#    print $fh_out "#!", ($self->opt('path.perl') || '/usr/bin/perl'), " -w\n\n" if $isExe;
#
#    print $fh_out "#===================================================================================\n";
#    print $fh_out "# GENERATED AUTOMATICALLY BY MET\@PAGE SOFTWARE, DO NOT CHANGE THIS SCRIPT MANUALLY \n";
#    print $fh_out "#===================================================================================\n\n";
#
#    print $fh_out "use lib qw(", join(' ', @{$self->opt('dir.inc')}), ");\n" if $isExe && $self->opt('dir.inc');
#
#    # требуется вывод заголовка HTTP/1.1
#    print $fh_out "use MetaPage::HTTP;\n" if $isExe && $self->opt('http.header');
#
#    print $fh_out join("\n", @{$self->pre});
#
#    print $fh_out "\nprint MetaPage::HTTP::header();\n" if $isExe && $self->opt('http.header');
#    print $fh_out html_start, join('', @{$self->text}), html_end;
#
#    print $fh_out join("\n", @{$self->post});
#
#    print $fh_out "\n0;\n" if $isExe;     # тип - perl-скрипт
#    print $fh_out "\n1;\n" unless $isExe; # тип - perl-модуль
#
#    1;
#}

1;
