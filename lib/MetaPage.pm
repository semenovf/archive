package MetaPage;
use MetaPage::HandlerFactory;
use MetaPage::Parser;
use MetaPage::Exception;
use strict;
use warnings;

use base qw(JQ::App);

use constant TEMPLATES_DIR => 'tpl';

__PACKAGE__->mk_accessors(qw(parser tmplMainPath tmplAppPath tmplPrefsPath));


sub new
{
    my $class = shift or die;
    my $self = $class->SUPER::new(@_);
    logging_debug( $self->pref('verbose') );
    logging_encoding( $self->pref('logger_encoding') ) if defined $self->pref('logger_encoding');
    default_encoding( $self->pref('encoding') ) if defined $self->pref('encoding');
    $self->parser( MetaPage::Parser->new() );
    MetaPage::HandlerFactory->handlersFor( $self->parser );

    $self->tmplMainPath (
        $self->isWebApp
            ? File::Spec->catfile(TEMPLATES_DIR, 'index.pl.tpl')
            : File::Spec->catfile(TEMPLATES_DIR, 'main.pl.tpl')
    );

    $self->tmplAppPath (
        $self->isWebApp
            ? File::Spec->catfile(TEMPLATES_DIR, 'WebApp.pm.tpl')
            : File::Spec->catfile(TEMPLATES_DIR, 'App.pm.tpl')
    );

    $self->tmplPrefsPath( File::Spec->catfile(TEMPLATES_DIR, 'Prefs.pm.tpl') );

    return bless $self, $class;
}

sub isWebApp
{
    my $self = shift or die;
    return $self->pref('webapp') ? 1 : 0;
}

sub exec
{
    my $self = shift or die;
    my $parser = $self->parser;

    SWITCH: {
        $self->pref('project') && do {
            $self->genProjectTemplate();
            last SWITCH;
        };

        $self->pref('create') && do {
            JQ::Exception::InvalidValue->throw('project must be specified') unless $self->pref('project');

            eval {
                $self->makeProjectTree();
                $self->genSamples();
                    #$self->genSampleApp();
                    #$self->genSamplePrefs();
                    #foreach my $file (@ARGV) {
                    #    $parser->parsefile( $file, ErrorContext=>3 );
                    #    $parser->renderCode();
                    #}
            }; if( $@ ) {
                die $@;
            #       $self->purgeProjectTree();
            }
            last SWITCH;
        };
    }
}

sub makeProjectTree
{
    my $self = shift or die;

    my $project_dir = File::Spec->catfile($self->pref('base-dir'), $self->pref('project'));
    JQ::Exception::IO->throw( sprintf('%s: project directory already exists', $project_dir) )
        if -e $project_dir;

    make_path( $project_dir, { verbose=>0, error=>\my $errors} );
    if( @$errors ) {
        my $plain_err_text = 'make path error(s) occured:';
        foreach( @$errors ) {
            $plain_err_text .= '\n\t';
            my ( $f, $e ) = %$_;
            if( $f eq '' ) {
                $plain_err_text .= $e;
            } else {
                $plain_err_text .= $f.': '.$e;
            }
        }
        JQ::Exception::IO->throw( sprintf('%s: %s', $project_dir, $plain_err_text) );
    }

    return 1;
}


sub purgeProjectTree
{
    my $self = shift or die;
#    my $path = shift or die ExArgumentExpected;
    #-e $path and die ExPathExists;
}

# usage:
# $self->_doCommonReplacement( $string_ref )
#
sub _doCommonReplacement
{
    my $self = shift or die;
    my $text = shift or die;
    #my ${_package_name_}    = $self->pref('project') or JQ::Exception::InvalidValue->throw( 'project' );
    #my ${_program_name_}    = $self->pref('program') or JQ::Exception::InvalidValue->throw( 'program' );
    #my ${_program_version_} = $self->pref('version');
    #my ${_program_author_}  = $self->pref('author');

    my (
        ${_package_name_},
        ${_program_name_},
        ${_program_version_},
        ${_program_author_} ) = map { $self->pref($_) or JQ::Exception::InvalidValue->throw( sprintf("invalid value for preference '%s'", $_) ); }
            qw(project program version author);

    if( ref $text ) {
        die unless ref $text eq 'SCALAR';
        return eval "\"$$text\"";
    }
    return eval "\"$text\"";
}

# Generates samples from templates
#
sub genSamples
{
    my $self = shift or die;
    local $/;

    foreach ((
        $self->tmplMainPath,
        $self->tmplAppPath,
        $self->tmplPrefsPath
    )) {
        open my $fh, '<', $_
            or JQ::Exception::IO->throw( sprintf('%s: %s', $_, $!) );
        my $text = <$fh>;
        close $fh;
        print $self->_doCommonReplacement(\$text);
    }
}


sub genProjectTemplate
{
    my $self = shift or die;
    my $fh = shift || *STDOUT;

    my (
        $_perlbin_,
        $_basedir_,
        $_projectname_,
        $_bootstrap_,
        $_version_,
        $_author_,
        $_apptype_
    ) = map { (my $v = $_) =~ s/\\/\\\\/g; $v; } (
        JQ::Utils::Perl::perl_interpreter,
        'need_to_define_basedir_for_project',
        'project_name',
        'bootstrap_name.pl',
        '0.0.1',
        getlogin() || getpwuid($<) || 'your_author_name',
        'consoleapp'
    );

    $_apptype_ = 'webapp' if $self->prefs('webapp');

    my $xml = <<EOXML;
<?xml version=\\"1.0\\"?>
<metapage>
    <${_apptype_}>
        <perlbin>${_perlbin_}</perlbin>
        <basedir>${_basedir_}<basedir>
        <project>${_projectname_}</project>
        <bootstrap>${_bootstrap_}</bootstrap>
        <version>${_version_}</version>
        <author>${_author_}</author>
    </${_apptype_}>
</metapage>
EOXML

    $xml = eval "\"$xml\"";

    print $fh $xml;
}

1;
