<?php
//phpinfo();
$mem = new Memcache;
$mem -> connect('localhost',11211);
$key = "test1";
$mem -> set($key,1,0,300);
$key = "test2";
$mem -> set($key,2,0,300);
$key = "test3";
$mem -> set($key,3,0,300);
$result = $mem ->getStats();
print_r($result); echo "<br/>";
$result = $mem -> getVersion();
print_r($result); echo "<br/>";
$result = $mem -> getServerStatus('localhost',11211);
print_r($result); echo "<br/>";

//$m = new Memcache();
//$m->addServer('localhost', 11211);
//
//$m->set('int', 99);
//$m->set('string', 'a simple string');
//$m->set('array', array(11, 12));
//
//$m->getDelayed(array('int', 'array'), true);
//var_dump($m->fetchAll());

//$memcache_obj = new Memcache;
//$memcache_obj->connect('localhost', 11211);
//$memcache_obj -> set('some_key' , 1 , 0 , 3000);
//$memcache_obj -> set('second_key' , 1 , 0 , 3000);
//$key = array(
//	'some_key',
//	'second_key',
//);
//$var = $memcache_obj->get($key);
//print_r($var);

?>