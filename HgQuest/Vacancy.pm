package HgQuest::Vacancy;
use strict;
use String::CRC32;
use cwt::Utils;

#require Exporter;
#our @ISA = qw(Exporter);
#our @EXPORT = qw( jq_prefix_contact_name
#    jq_prefix_contact_phone
#    jq_prefix_contact_fax
#    jq_prefix_contact_email
#    jq_prefix_contact_site
#    jq_prefix_contact_addr
#    jq_prefix_contact_unknown
#    jq_vptype );
#our @EXPORT_OK = qw(JQC_JQID_INVALID );



#   ������������� ����� ������ ������� ���� (�����)- ������ �������� �����
#   ������������� ��������� ����� ( ����� ������������, ������������� � ��� ���-������)
#   ��� ����������� ��������� ������ � ������ �������������
#

sub JQC_JQID_INVALID () {'JQID-000000-__INVALID_'}

# ����������� � ������� ��������
#
my %_CONSTANTS = (
# ��������� (����) ��������
    JQC_JQID		=>  0,	 # ���������� ������������� (��. JobQuest::JQConst::jq_JQID_gen) (STR)
    JQC_BRANCH 		=>  1,   # �������			    (VOC)
    JQC_POST		=>  2,   # ���������			(STR)
    JQC_CITY    	=>  3,   # ������ (�����)		(STR)
    JQC_DATE    	=>  4,   # ���� ���������� �������� (DATE, YYYYMMDD) see JQ::Utils::jq_date_str
    JQC_SEX     	=>  5,   # ��� (�/�)			(VOC)
    JQC_EDU     	=>  6,   # �����������			(VOC)
    JQC_EXP     	=>  7,   # ���� ������ � �����  (NUMBER)
    JQC_AGEFROM    	=>  8,   # ������� ��			(NUMBER)
    JQC_AGETO    	=>  9,   # ������� ��			(NUMBER)
    JQC_SALARY  	=> 10,   # ���������� ����� 	(NUMBER)
    JQC_CUR     	=> 11,   # ������ (rur/usd/eur) (VOC)
    JQC_SCHED   	=> 12,   # ������ �����			(VOC)
    JQC_EMP     	=> 13,   # ���������			(VOC)
    JQC_OFFER   	=> 14,   # ��� ��������� ������ (STR) �� ����, ������ ���� ������ �� ������ �����������/�����������
    JQC_TEXT		=> 15,	 # ����� �������� (�������� ��������) (TEXT)
    JQC_CONTACT     => 16,   # ���������� ���������� (TEXT) - ��� ������ ��������� ����� JQC_CONTACT_XXX
    JQC_EXTRA       => 17,   # �������������� �������� � �������� (TEXT)
    JQC_SRCURL		=> 18,   # �������� (URL) ��������          (STR - url)
    JQC_SIZE		=> 19,	 # ���������� ���������� ��������
  
    # �������� ��������� ����������
    #

    # ������� (BRANCH)
    #
    JQC_BRANCH_UNKNOWN	    => 0,   # ������ ����� ������������
    JQC_BRANCH_IT 	        => 1,   # ������������� ����������//��������//�������
    JQC_BRANCH_TRADE   	    => 2,   # ��������//���������//�����//���//�������//���������
    JQC_BRANCH_PR 	        => 3,   # ���������//�������//PR
    JQC_BRANCH_MEDICINE     => 4,   # ��������//������������
    JQC_BRANCH_EDUCATION    => 5,   # �����//�����������//����������
    JQC_BRANCH_MANUFACTURE  => 6,   # ������������//��������������//������ �����
    JQC_BRANCH_MAINTENANCE  => 7,   # �����������//���//���
    JQC_BRANCH_MASSMEDIA    => 8,   # ���//������������//����������
    JQC_BRANCH_BUILDING     => 9,   # ������������//�������������//�����������
    JQC_BRANCH_SERVICE 	    => 10,  # ����� �����//������//���������//���������
    JQC_BRANCH_TRANSPORT    => 11,  # ���������//����������
    JQC_BRANCH_FINANCE 	    => 12,  # �������//����//�����������//����������//������ 
    JQC_BRANCH_LAW 	        => 13,  # �������������//�����//����������
    JQC_BRANCH_ENTMNT       => 14,  # ���������//�����������//����-�����//�����//������
    JQC_BRANCH_FARMING      => 15,  # �������� ���������
    JQC_BRANCH_PUBLIC       => 16,  # ��������������� ������
    JQC_BRANCH_STAFF        => 17 , # ���������� ����������//��������
    JQC_BRANCH_INSUR        => 18 , # �����������
    JQC_BRANCH_CHIEF        => 19 , # ������������

    # ��� (Sex)
    JQC_SEX_UNKNOWN         => 0,	# �� ������
    JQC_SEX_ANY   	        => 1,	# �����
    JQC_SEX_MALE   	        => 2,   # �������
    JQC_SEX_FEMALE	        => 3,   # �������

    # ����������� (Education)
    JQC_EDU_UNKNOWN         => 0, #
    JQC_EDU_ANY             => 1, #
    JQC_EDU_SECONDARY	    => 2, # �������
    JQC_EDU_SPECIAL	        => 3, # ������� �����������
    JQC_EDU_STUDENT	        => 4, # �������
    JQC_EDU_INTERNAL        => 5, # ������� ������ ���������
    JQC_EDU_EXTERNAL	    => 6, # ������� �������� ���������
    JQC_EDU_UNFINISHED      => 7, # �������������
    JQC_EDU_HIGH            => 8, # ������
    
    # ������
    JQC_CUR_UNKNOWN         => 0,
    JQC_CUR_RUR		        => 1,
    JQC_CUR_USD		        => 2,
    JQC_CUR_EUR	            => 3,
    
    # ������ ������
    JQC_SCHED_UNKNOWN       => 0,   # �� ��������
    JQC_SCHED_ANY           => 1,	# �����
    JQC_SCHED_FULL          => 2,	# ������ ������� ����
    JQC_SCHED_FREE          => 3,   # ��������� ������
    JQC_SCHED_CONTRACT      => 4,   # ��������
    JQC_SCHED_PART          => 5,   # �������� ������� ����
    JQC_SCHED_SHIFT         => 6,   # �����
    JQC_SCHED_GANG          => 7,   # ��������
    
    # ��������� (Employment)
    JQC_EMP_UNKNOWN  	    => 0,  # �� ��������
    JQC_EMP_ANY   	        => 1,  # �����
    JQC_EMP_FULL   	        => 2,  # ������
    JQC_EMP_PART   	        => 3,  # ���������
    JQC_EMP_PLURAL 	        => 4,  # �� ���������������� (pluralistically)
    JQC_EMP_TEMP            => 5,  # ���������
    
    # Invalid values for parameters
    JQC_INVALID_POST        => '_INVALID_POST_',
    JQC_INVALID_SITE        => '_INVALID_SITE_',
    JQC_INVALID_OFFER       => '_INVALID_OFFER_',
    JQC_INVALID_TEXT        => '_INVALID_TEXT_',
    JQC_INVALID_DATE        => 19000101,
    JQC_INVALID_SALARY      => 0,
    #  JQC_INVALID_CUR     => -1,
    JQC_INVALID_SEX         => -1,
    #  JQC_INVALID_EDU     => -1,
    JQC_INVALID_AGEFROM     => -1,
    JQC_INVALID_AGETO       => -1,
    JQC_INVALID_EXP         => -1,
    #  JQC_INVALID_SCHED   => -1
);

#print %_CONSTANTS, "\n";
my $constants = '';
my ($k,$v);	# constant name, constant value
#while(($k, $v) = each(%_CONSTANTS) ) {
foreach $k (keys(%_CONSTANTS)) {
  $v = $_CONSTANTS{$k};
  $constants .= "sub $k () { $v }\t";
  $constants .= "push(\@EXPORT, '$k');\n";
}
#print $constants;
eval $constants;
die $@ if $@;

#
# � �������� ������� ������ ������ ��� �������� ������ � ����������� ������
#
sub new {
  my $class = shift;
  my $self = bless {
    'data'=>[
      $_CONSTANTS{JQC_JQID_INVALID},    # JQC_JQID
      $_CONSTANTS{JQC_BRANCH_UNKNOWN},  # JQC_BRANCH
      '',                               # JQC_POST
      '',                               # JQC_CITY
      $_CONSTANTS{JQC_INVALID_DATE},    # JQC_DATE
      $_CONSTANTS{JQC_SEX_UNKNOWN},     # JQC_SEX
      $_CONSTANTS{JQC_EDU_UNKNOWN},     # JQC_EDU
      0,                                # JQC_EXP
      0,                                # JQC_AGEFROM
      0,                                # JQC_AGETO
      0,                                # JQC_SALARY
      $_CONSTANTS{JQC_CUR_UNKNOWN},     # JQC_CUR
      $_CONSTANTS{JQC_SCHED_UNKNOWN},   # JQC_SCHED
      $_CONSTANTS{JQC_EMP_UNKNOWN},     # JQC_EMP
      '',                               # JQC_OFFER
      '',                               # JQC_TEXT
      '',                               # JQC_CONTACT
      '',                               # JQC_EXTRA
      '']                               # JQC_SRCURL
  }, $class;
  return $self;
}

# ������������� �������� ���������
sub set
{ $_[0]->{'data'}->[$_[1]] = $_[2]; }

# ���������� �������� ���������. ��� ���� ��� ���������� ��������� ������������ ���
#
sub get
{
    $_[0]->{'data'}->[$_[1]];
}


#
# ���������� ��������� �������� ���������. ���� ��� ��������� ��������,
# �� ���������� �� �������� (�� ���)
#
sub getv
{
    my $self = shift;
    my $p = shift;    # ������ ���������
    return jq_vocval( $p, $self->{'data'}->[$p] ) if jq_vptype($p) eq 'voc';
    return jq_date_str($self->{'data'}->[$p]) if jq_vptype($p) eq 'date';
    return $self->{'data'}->[$p]; 
}

=item $vacancy->complete()
  ����������� ��������. ��������, � ��� �����,
  ��������� ����������� �������������� �������� JQID
=cut

sub complete
{
    my $self = shift;
    $self->{'data'}->[$_CONSTANTS{JQC_JQID}] = _JQID_gen(
        -post    => $self->{'data'}->[$_CONSTANTS{JQC_POST}],
        -branch  => $self->{'data'}->[$_CONSTANTS{JQC_BRANCH}],
        -sex     => $self->{'data'}->[$_CONSTANTS{JQC_SEX}],
        -edu     => $self->{'data'}->[$_CONSTANTS{JQC_EDU}],
        -sched   => $self->{'data'}->[$_CONSTANTS{JQC_SCHED}],
        -emp     => $self->{'data'}->[$_CONSTANTS{JQC_EMP}]);
}


#
# ������������ ���������� (RU - win1251)
sub _VPNAME_RU_WIN1251
{
    my @vpname = (
    'jqid',
    '�������',
    '���������',
    '������/�����',
    '���� ���������� ��������',
    '���',
    '�����������',
    '���� ������ (���)',
    '������� (��)',
    '������� (��)',
    '���������� �����',
    '������',
    '������ �����',
    '���������',
    '��������� ��������',
    '����� ��������',
    '���������� ����������',
    '�������������� ��������',
    '��������');
    
    return wantarray ? @vpname : $vpname[shift];
}


sub _VOCVAL_RU_WIN1251
{
    my $a = [
        [], # JQC_JQID
        
        ['������ ����� ������������',
        '�������������� ����������//��������//�������',
        '��������//���������//�����//���//�������//���������',
        '���������//�������//PR',
        '��������//������������',
        '�����//�����������//����������',
        '������������//��������������//������ �����',
        '�����������//���//���',
        '���//������������//����������',
        '������������//�������������//�����������',
        '����� �����//������//���������//���������',
        '���������//����������',
        '�������//����//�����������//����������//������',
        '�������������//�����//����������',
        '���������//�����������//����-�����//�����//������',
        '�������� ���������',
        '��������������� ������',
        '���������� ����������//��������',
        '�����������',
        '������������'], # JQC_BRANCH

        [], # JQC_POST
        [], # JQC_CITY
        [], # JQC_DATE
    
        ['�� ������', '�����', '�������', '�������'], # JQC_SEX
    
        ['�� �������',
        '�����',
        '�������',
        '������� �����������',
        '�������',
        '������� ������ ���������',
        '������� �������� ���������',
        '�������������',
        '������'],  # JQC_EDU
        
        [],         # JQC_EXP
        [],         # JQC_AGEFROM
        [],         # JQC_AGETO
        [],         # JQC_SALARY
    
        ['unk', 'RUR', 'USD', 'EUR'], # JQC_CUR
    
        ['�� ������',
        '�����',
        '������ ������� ����',
        '��������� ������',
        '��������',
        '�������� ������� ����',
        '��������',
        '��������'], # JQC_SCHED
    
        ['�� �������',
        '�����',
        '������',
        '���������',
        '�� ����������������',
        '���������'], # JQC_EMP
        
        [], # JQC_OFFER
        [], # JQC_TEXT
        [], # JQC_CONTACT
        [], # JQC_EXTRA
        []  # JQC_SRCURL
    ];
    
    return wantarray ? @{$a->[$_[0]]} : $a->[$_[0]]->[$_[1]];
}

# ���������� ��� ��������� ��������
#
# ���������� ����
# jqid - ���������� ������������� ��������
# date
# str\d*
# text
# int\d*
# num\d*(\.\d\d*)? 
# voc\d*
# uri
sub jq_vptype($)
{
    return [
	'jqid',
	'voc',     # JQC_BRANCH
	'str128',  # JQC_POST
	'str128',   # JQC_CITY
	'date',    # JQC_DATE
	'voc',     # JQC_SEX
	'voc',     # JQC_EDU
	'int',     # JQC_EXP
	'int',     # JQC_AGEFROM
	'int',     # JQC_AGETO
	'num10.2', # JQC_SALARY
	'voc',     # JQC_CUR
	'voc',     # JQC_SCHED
	'voc',     # JQC_EMP
	'str128',  # JQC_OFFER
	'text',    # JQC_TEXT
	'text',    # JQC_CONTACT
        'text',    # JQC_EXTRA
        'uri'      # JQC_SRCURL
    ]->[$_[0]];
}

# ������������� ������������ �������
#
sub jq_branch_abbr
{
    my $short = [
        'unk', # JQC_BRANCH_UNKNOWN
        'itt', # JQC_BRANCH_IT
        'trd', # JQC_BRANCH_TRADE
        'prm', # JQC_BRANCH_PR
        'med', # JQC_BRANCH_MEDICINE
        'edu', # JQC_BRANCH_EDUCATION
        'man', # JQC_BRANCH_MANUFACTURE
        'mnt', # JQC_BRANCH_MAINTENANCE
        'mas', # JQC_BRANCH_MASSMEDIA
        'bld', # JQC_BRANCH_BUILDING
        'srv', # JQC_BRANCH_SERVICE
        'trn', # JQC_BRANCH_TRANSPORT
        'fin', # JQC_BRANCH_FINANCE
        'law', # JQC_BRANCH_LAW
        'ent', # JQC_BRANCH_ENTMNT
        'far', # JQC_BRANCH_FARMING
        'pub', # JQC_BRANCH_PUBLIC
        'stf', # JQC_BRANCH_STAFF
        'ins', # JQC_BRANCH_INSUR
        'chf'];# JQC_BRANCH_CHIEF
    return wantarray ? @$short : $short->[$_[0]];
}


sub jq_prefix_contact_name  { return '�������::����:'.$_[0]."\n"; }
sub jq_prefix_contact_phone { return '�������::�������:'.$_[0]."\n"; }
sub jq_prefix_contact_fax   { return '�������::����:'.$_[0]."\n"; }
sub jq_prefix_contact_email { return '�������::E-mail:'.$_[0]."\n"; }
sub jq_prefix_contact_site  { return '�������::����:'.$_[0]."\n"; }
sub jq_prefix_contact_addr  { return '�������::�����:'.$_[0]."\n"; }
sub jq_prefix_contact_unknown  { return '�������::'.$_[0].':'.$_[1]."\n"; }
 
# ������ �������� ������������� ���������� �������������
# JQID-AABCDE-FFFFFFFFFF
#	���
#	AA	- ������� (��������, 09 - ������������)
#	B	- ���
#	C	- �����������
#	D	- ������ �����
#	E	- ���������
#	FFFFFFFF - CRC32 (Hex) - ��������� (������ �������, �����, �����, ������ �������������)
#
sub _JQID_gen
{
  my %args = @_;
  my $post    = defined($args{-post}) ? $args{-post} : '';
  
  $post =~ tr/a-zA-Z0-9�������������������������������������Ũ��������������������������_-//cd;
  
  return sprintf("JQID-%02d%d%d%d%d-%08X",
	defined($args{-branch}) ? $args{-branch} : $_CONSTANTS{JQC_BRANCH_UNKNOWN},
	defined($args{-sex})    ? $args{-sex}    : $_CONSTANTS{JQC_SEX_UNKNOWN},
	defined($args{-edu})    ? $args{-edu}    : $_CONSTANTS{JQC_EDU_UNKNOWN},
	defined($args{-sched})  ? $args{-sched}  : $_CONSTANTS{JQC_SCHED_UNKNOWN},
	defined($args{-emp})    ? $args{-emp}    : $_CONSTANTS{JQC_EMP_UNKNOWN},
	crc32($post, length($post)) );
}

*jq_vpname = *_VPNAME_RU_WIN1251;
*jq_vocval = *_VOCVAL_RU_WIN1251;

1;
