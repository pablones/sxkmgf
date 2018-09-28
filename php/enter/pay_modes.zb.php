<?php
/**
 * �鱦��ֵ�ӿ����
 * @author King.L
 */

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');
/*����֧����*/
require_once(ROOT_PATH . 'class/mode_class.php');
/* ���������ļ� */
require_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/user.php');

//����ֵ˵��
$return = array(
    'payum_error'=> 'PayNum ����',
    'mode_error'=> 'Mode ����',
    'paygold_error'=> 'PayGold ����',
    'paytime_error'=> 'PayTime ����',
    'paynum_exist'=> 'PayNum �Ѿ�����',
    'username_error'=> 'UserName ����',
    'ticket_error'=> 'Ticket ����',
    'bad_request'=> '�Ƿ�����',
    'servername_error'=> '�������Ŵ���',
    'account_error'=> '���˻�������',
    'ok'=> '��ֵ�ɹ�',
    'failed'=> '��ֵʧ��',
);



//���ʵ�IP�޶�
//�Ƿ��͵��
//TODO $allowIpArrIn ������
if(in_array($_SG['ip'],$allowIpArrIn)){
    define('GIVE_GOLD3',TRUE); //��
}else{
    define('GIVE_GOLD3',FALSE); //����
}
if(!in_array($_SG['ip'],$allowIpArrIn)) exit('403');//��Ȩ�޲���

$path='../data/ptlog/zblog.php';


$pay = new gamePay($_GET);
$pay->ckPayNum();//���PayNum
if($pay->Mode != 1) eixt('mode_error');
if(empty($pay->ServerName)) exit('servername_error'); //�������Ŵ���
if(empty($pay->PayToUser)) exit('username_error');
if($pay->PayGold <= 0) exit('paygold_error');
if($pay->PayTime <= 0) exit('paytime_error');
if(empty($pay->ticket)) exit('ticket_error');
if(!$pay->ckTicket()) exit('bad_request');
$pay->ZhuBaoPay();

?>