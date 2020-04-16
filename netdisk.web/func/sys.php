<?php

function Alert($inf){
    echo "<script>alert('$inf');</script>";
}
function AlertReturn($inf){
    echo "<script>alert('$inf');history.go(-1);</script>";
}
function AlertBack($inf,$url){
    echo "<script>alert('$inf');</script>";
    header("refresh:0;../page/$url");
}


function _checkPwLevel($pw){
        if(empty($pw)){
            return 0;
        }

        $number="0-9";
        $lower="a-z";
        $cap="A-Z";
        $others="\x21-\x2f,\x3a-\x40,\x5b-\x60,\x7b-\x7e";

        $pattern['weak'] = '/((^[0-9]{12,})|(^[a-z]{12,})|(^[A-Z]{12,}))$/';;
        $pattern['middle'] = '/((^[0-9,a-z]{12,})|(^[0-9,A-Z]{12,})|(^[a-z,A-Z]{12,})|([\x21-\x2f,\x3a-\x40,\x5b-\x60,\x7b-\x7e,0-9]{12,})|([\x21-\x2f,\x3a-\x40,\x5b-\x60,\x7b-\x7e,a-z]{12,})|([\x21-\x2f,\x3a-\x40,\x5b-\x60,\x7b-\x7e,A-Z]{12,}))$/';
        $pattern['strong'] = '/^[\x21-\x7e,A-Za-z0-9]{12,}/';
    
        $key ='';
    
        foreach($pattern as $k=>$v){
            $res = preg_match($v,$pw);
            if($res){
                $key = $k;
                break;
            }
        }
    
        switch ($key){
    
            case 'weak':
                return 3;
            case 'middle':
                return 2;
            case 'strong':
                return 1;
            default:
                return 0;
        }
}

function myhash($username,$password) {    
        // salt encryption
        //$salt = md5($username);     
        //$password=md5($password).$salt;
        $password=md5($password);   // md5 hash
        return $password;       // return hash 
}



// gbk
function unescape1($str) {
    $str = rawurldecode($str);
    preg_match_all("/%u.{4}|&#x.{4};|&#d+;|.+/U",$str,$r);
    $ar = $r[0];
    foreach($ar as $k=>$v) {
        if(substr($v,0,2) == "%u")
            $ar[$k] = iconv("UCS-2","GBK",pack("H4",substr($v,-4)));
        elseif(substr($v,0,3) == "&#x")
            $ar[$k] = iconv("UCS-2","GBK",pack("H4",substr($v,3,-1)));
        elseif(substr($v,0,2) == "&#") {
            $ar[$k] = iconv("UCS-2","GBK",pack("n",substr($v,2,-1)));
        }
    }
    return join("",$ar);
} 
function unescape($str){
$ret = '';
$len = strlen($str);
for ($i = 0; $i < $len; $i++){
if ($str[$i] == '%' && $str[$i+1] == 'u'){
$val = hexdec(substr($str, $i+2, 4));
if ($val < 0x7f) $ret .= chr($val);
else if($val < 0x800) $ret .= chr(0xc0|($val>>6)).chr(0x80|($val&0x3f));
else $ret .= chr(0xe0|($val>>12)).chr(0x80|(($val>>6)&0x3f)).chr(0x80|($val&0x3f));
$i += 5;
}
else if ($str[$i] == '%'){
$ret .= urldecode(substr($str, $i, 3));
$i += 2;
}
else $ret .= $str[$i];
}
return $ret;
} 
?>
