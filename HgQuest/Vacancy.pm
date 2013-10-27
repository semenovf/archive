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



#   Дополнительно нужно ввести понятие тэга (ключи)- Каждой вакансии может
#   соответствует несколько тэгов ( сфера деятельности, специализация и еще что-нибудь)
#   для возможности внедрения поиска с учетом релевантности
#

sub JQC_JQID_INVALID () {'JQID-000000-__INVALID_'}

# Определение и экспорт констант
#
my %_CONSTANTS = (
# Параметры (поля) вакансии
    JQC_JQID		=>  0,	 # уникальный идентификатор (см. JobQuest::JQConst::jq_JQID_gen) (STR)
    JQC_BRANCH 		=>  1,   # отрасль			    (VOC)
    JQC_POST		=>  2,   # должность			(STR)
    JQC_CITY    	=>  3,   # регион (город)		(STR)
    JQC_DATE    	=>  4,   # дата размещения вакансии (DATE, YYYYMMDD) see JQ::Utils::jq_date_str
    JQC_SEX     	=>  5,   # пол (м/ж)			(VOC)
    JQC_EDU     	=>  6,   # образование			(VOC)
    JQC_EXP     	=>  7,   # опыт работы в годах  (NUMBER)
    JQC_AGEFROM    	=>  8,   # возраст от			(NUMBER)
    JQC_AGETO    	=>  9,   # возраст до			(NUMBER)
    JQC_SALARY  	=> 10,   # заработная плата 	(NUMBER)
    JQC_CUR     	=> 11,   # валюта (rur/usd/eur) (VOC)
    JQC_SCHED   	=> 12,   # график работ			(VOC)
    JQC_EMP     	=> 13,   # занятость			(VOC)
    JQC_OFFER   	=> 14,   # кто разместил заявку (STR) По идее, должна быть ссылка на объект Предприятие/Организация
    JQC_TEXT		=> 15,	 # текст вакансии (описание вакансии) (TEXT)
    JQC_CONTACT     => 16,   # контактная информация (TEXT) - для замены отдельных полей JQC_CONTACT_XXX
    JQC_EXTRA       => 17,   # дополнительные сведения о вакансии (TEXT)
    JQC_SRCURL		=> 18,   # источник (URL) вакансии          (STR - url)
    JQC_SIZE		=> 19,	 # количество параметров вакансии
  
    # Значения словарных параметров
    #

    # Отрасль (BRANCH)
    #
    JQC_BRANCH_UNKNOWN	    => 0,   # Прочие сферы деятельности
    JQC_BRANCH_IT 	        => 1,   # Информционные технологии//Интернет//Телеком
    JQC_BRANCH_TRADE   	    => 2,   # Торговля//Логистика//Склад//ВЭД//Закупки//Снабжение
    JQC_BRANCH_PR 	        => 3,   # Маркетинг//Реклама//PR
    JQC_BRANCH_MEDICINE     => 4,   # Медицина//Фармацевтика
    JQC_BRANCH_EDUCATION    => 5,   # Наука//Образование//Воспитание
    JQC_BRANCH_MANUFACTURE  => 6,   # Производство//Промышленность//Добыча сырья
    JQC_BRANCH_MAINTENANCE  => 7,   # Секретариат//АХЧ//АХО
    JQC_BRANCH_MASSMEDIA    => 8,   # СМИ//Издательство//Полиграфия
    JQC_BRANCH_BUILDING     => 9,   # Недвижимость//Строительство//Архитектура
    JQC_BRANCH_SERVICE 	    => 10,  # Сфера услуг//Охрана//Гостиницы//Рестораны
    JQC_BRANCH_TRANSPORT    => 11,  # Транспорт//Автобизнес
    JQC_BRANCH_FINANCE 	    => 12,  # Финансы//Банк//Бухгалтерия//Инвестиции//Лизинг 
    JQC_BRANCH_LAW 	        => 13,  # Юриспруденция//Аудит//Консалтинг
    JQC_BRANCH_ENTMNT       => 14,  # Искусство//Развлечения//Масс-медиа//Спорт//Туризм
    JQC_BRANCH_FARMING      => 15,  # Сельское хозяйство
    JQC_BRANCH_PUBLIC       => 16,  # Государственная служба
    JQC_BRANCH_STAFF        => 17 , # Управление персоналом//Тренинги
    JQC_BRANCH_INSUR        => 18 , # Страхование
    JQC_BRANCH_CHIEF        => 19 , # Руководители

    # Пол (Sex)
    JQC_SEX_UNKNOWN         => 0,	# не указан
    JQC_SEX_ANY   	        => 1,	# любой
    JQC_SEX_MALE   	        => 2,   # мужской
    JQC_SEX_FEMALE	        => 3,   # женский

    # Образование (Education)
    JQC_EDU_UNKNOWN         => 0, #
    JQC_EDU_ANY             => 1, #
    JQC_EDU_SECONDARY	    => 2, # среднее
    JQC_EDU_SPECIAL	        => 3, # среднее специальное
    JQC_EDU_STUDENT	        => 4, # студент
    JQC_EDU_INTERNAL        => 5, # студент очного отделения
    JQC_EDU_EXTERNAL	    => 6, # студент заочного отделения
    JQC_EDU_UNFINISHED      => 7, # незаконченное
    JQC_EDU_HIGH            => 8, # высшее
    
    # Валюта
    JQC_CUR_UNKNOWN         => 0,
    JQC_CUR_RUR		        => 1,
    JQC_CUR_USD		        => 2,
    JQC_CUR_EUR	            => 3,
    
    # График работы
    JQC_SCHED_UNKNOWN       => 0,   # не известен
    JQC_SCHED_ANY           => 1,	# любой
    JQC_SCHED_FULL          => 2,	# полный рабочий день
    JQC_SCHED_FREE          => 3,   # свободный график
    JQC_SCHED_CONTRACT      => 4,   # контракт
    JQC_SCHED_PART          => 5,   # неполный рабочий день
    JQC_SCHED_SHIFT         => 6,   # вахта
    JQC_SCHED_GANG          => 7,   # посменно
    
    # Занятость (Employment)
    JQC_EMP_UNKNOWN  	    => 0,  # не известна
    JQC_EMP_ANY   	        => 1,  # любая
    JQC_EMP_FULL   	        => 2,  # полная
    JQC_EMP_PART   	        => 3,  # частичная
    JQC_EMP_PLURAL 	        => 4,  # по совместительству (pluralistically)
    JQC_EMP_TEMP            => 5,  # временная
    
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
# В качестве объекта выбран массив для удобства работы с хранилищами данных
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

# устанавливает значение параметра
sub set
{ $_[0]->{'data'}->[$_[1]] = $_[2]; }

# Возвращает значение параметра. При этом для словарного параметра возвращается код
#
sub get
{
    $_[0]->{'data'}->[$_[1]];
}


#
# Возвращает строковое значение параметра. Если это словарная величина,
# то возвращает ее значение (не код)
#
sub getv
{
    my $self = shift;
    my $p = shift;    # индекс параметра
    return jq_vocval( $p, $self->{'data'}->[$p] ) if jq_vptype($p) eq 'voc';
    return jq_date_str($self->{'data'}->[$p]) if jq_vptype($p) eq 'date';
    return $self->{'data'}->[$p]; 
}

=item $vacancy->complete()
  Финализация вакансии. Включает, в том числе,
  генерацию уникального идентификатора вакансии JQID
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
# наименования параметров (RU - win1251)
sub _VPNAME_RU_WIN1251
{
    my @vpname = (
    'jqid',
    'отрасль',
    'должность',
    'регион/город',
    'дата размещения вакансии',
    'пол',
    'образование',
    'опыт работы (лет)',
    'возраст (от)',
    'возраст (до)',
    'заработная плата',
    'валюта',
    'график работ',
    'занятость',
    'инициатор вакансии',
    'текст вакансии',
    'контактная информация',
    'дополнительные сведения',
    'источник');
    
    return wantarray ? @vpname : $vpname[shift];
}


sub _VOCVAL_RU_WIN1251
{
    my $a = [
        [], # JQC_JQID
        
        ['Прочие сферы деятельности',
        'Информационные технологии//Интернет//Телеком',
        'Торговля//Логистика//Склад//ВЭД//Закупки//Снабжение',
        'Маркетинг//Реклама//PR',
        'Медицина//Фармацевтика',
        'Наука//Образование//Воспитание',
        'Производство//Промышленность//Добыча сырья',
        'Секретариат//АХЧ//АХО',
        'СМИ//Издательство//Полиграфия',
        'Недвижимость//Строительство//Архитектура',
        'Сфера услуг//Охрана//Гостиницы//Рестораны',
        'Транспорт//Автобизнес',
        'Финансы//Банк//Бухгалтерия//Инвестиции//Лизинг',
        'Юриспруденция//Аудит//Консалтинг',
        'Искусство//Развлечения//Масс-медиа//Спорт//Туризм',
        'Сельское хозяйство',
        'Государственная служба',
        'Управление персоналом//Тренинги',
        'Страхование',
        'Руководители'], # JQC_BRANCH

        [], # JQC_POST
        [], # JQC_CITY
        [], # JQC_DATE
    
        ['не указан', 'любой', 'мужской', 'женский'], # JQC_SEX
    
        ['не указано',
        'любое',
        'среднее',
        'среднее специальное',
        'студент',
        'студент очного отделения',
        'студент заочного отделения',
        'незаконченное',
        'высшее'],  # JQC_EDU
        
        [],         # JQC_EXP
        [],         # JQC_AGEFROM
        [],         # JQC_AGETO
        [],         # JQC_SALARY
    
        ['unk', 'RUR', 'USD', 'EUR'], # JQC_CUR
    
        ['не указан',
        'любой',
        'полный рабочий день',
        'свободный график',
        'контракт',
        'неполный рабочий день',
        'вахтовый',
        'посменно'], # JQC_SCHED
    
        ['не указана',
        'любая',
        'полная',
        'частичная',
        'по совместительству',
        'временная'], # JQC_EMP
        
        [], # JQC_OFFER
        [], # JQC_TEXT
        [], # JQC_CONTACT
        [], # JQC_EXTRA
        []  # JQC_SRCURL
    ];
    
    return wantarray ? @{$a->[$_[0]]} : $a->[$_[0]]->[$_[1]];
}

# Возвращает тип параметра вакансии
#
# Допустимые типы
# jqid - уникальный идентификатор вакансии
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

# Трехбуквенная аббревиатура отрасли
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


sub jq_prefix_contact_name  { return 'Контакт::Лицо:'.$_[0]."\n"; }
sub jq_prefix_contact_phone { return 'Контакт::Телефон:'.$_[0]."\n"; }
sub jq_prefix_contact_fax   { return 'Контакт::Факс:'.$_[0]."\n"; }
sub jq_prefix_contact_email { return 'Контакт::E-mail:'.$_[0]."\n"; }
sub jq_prefix_contact_site  { return 'Контакт::Сайт:'.$_[0]."\n"; }
sub jq_prefix_contact_addr  { return 'Контакт::Адрес:'.$_[0]."\n"; }
sub jq_prefix_contact_unknown  { return 'Контакт::'.$_[0].':'.$_[1]."\n"; }
 
# Каждой вакансии соответствует уникальный идентификатор
# JQID-AABCDE-FFFFFFFFFF
#	где
#	AA	- отрасль (например, 09 - производство)
#	B	- пол
#	C	- образование
#	D	- график работ
#	E	- занятость
#	FFFFFFFF - CRC32 (Hex) - должность (только символы, цифры, дефис, нижнее подчеркивание)
#
sub _JQID_gen
{
  my %args = @_;
  my $post    = defined($args{-post}) ? $args{-post} : '';
  
  $post =~ tr/a-zA-Z0-9абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ_-//cd;
  
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
