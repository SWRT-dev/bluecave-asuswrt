<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<title><#587#> - Koolproxy</title>
<link rel="stylesheet" type="text/css" href="index_style.css">
<link rel="stylesheet" type="text/css" href="form_style.css">
<style>
p{
font-weight: bolder;
}
.disabled-feature-ihacklog {
display: none;
}
.contentM_qis{
position:absolute;
-webkit-border-radius: 5px;
-moz-border-radius: 5px;
border-radius: 5px;
z-index:200;
background-color:#2B373B;
display:none;
margin-left: 32%;
top: 250px;
}
</style>
<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/tmhist.js"></script>
<script language="JavaScript" type="text/javascript" src="/tmmenu.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/js/jquery.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script>
$(document).ready(function () {
var havecert="<% check_file("/jffs/adbyby/data/certs/ca.crt"); %>";
if(havecert=="0")
{
document.form.mycert.value = "尚未生成";
document.form.mycert.disabled = true;
}
$('#radio_adbyby_enable').iphoneSwitch(document.form.adbyby_enable.value,
function(){
document.form.adbyby_enable.value = "1";
},
function(){
document.form.adbyby_enable.value = "0";
}
);
$('#radio_adbyby_video_mode').iphoneSwitch(document.form.adbyby_video_mode.value,
function(){
document.form.adbyby_video_mode.value = "1";
},
function(){
document.form.adbyby_video_mode.value = "0";
}
);
$('#radio_adbyby_https_mode').iphoneSwitch(document.form.adbyby_https_mode.value,
function(){
document.form.adbyby_https_mode.value = "1";
},
function(){
document.form.adbyby_https_mode.value = "0";
}
);
$('#radio_adbyby_monitor').iphoneSwitch(document.form.adbyby_monitor.value,
function(){
document.form.adbyby_monitor.value = "1";
},
function(){
document.form.adbyby_monitor.value = "0";
}
);
$('.disabled.cert').click(function(){return false;});
if ($( "#kp_status span" ).hasClass("label-warning")) {
$('.cert').addClass('disabled');
}
});
</script>
<script>
function setRulesCheck(d1, d2, d3) {
str = "";
if (d3.checked == true) str = "1" + str;
else str = "0" + str;
if (d2.checked == true) str = "1" + str;
else str = "0" + str;
if (d1.checked == true) str = "1" + str;
else str = "0" + str;
return str;
}
function initial() {
show_menu();
document.form.adbyby_srcip.value = document.form.adbyby_srcip.value.replace(/,/g,"\r\n");
document.form.adbyby_httpsip.value = document.form.adbyby_httpsip.value.replace(/,/g,"\r\n");
hide_adb(<% nvram_match("adblock_mode", "0", "0"); %>);
}
function applyRule() {
document.form.adbyby_srcip.value = document.form.adbyby_srcip.value.replace(/\r\n/g,",");
document.form.adbyby_srcip.value = document.form.adbyby_srcip.value.replace(/\n/g,",");
document.form.adbyby_httpsip.value = document.form.adbyby_httpsip.value.replace(/\r\n/g,",");
document.form.adbyby_httpsip.value = document.form.adbyby_httpsip.value.replace(/\n/g,",");
showLoading();
document.form.submit();
}
function done_validating(action) {
refreshpage();
}
function hide_adb(_value){
if(_value==null)
_value=1;
showhide("kptr1", (_value == "1"));
showhide("kptr2", (_value == "1"));
showhide("kptr3", (_value == "1"));
}
</script>
</head>
<body onload="initial();" onunLoad="return unload_body();">
<div id="TopBanner"></div>
<div id="hiddenMask" class="popup_bg" style="z-index:10000;">
<table cellpadding="5" cellspacing="0" id="dr_sweet_advise" class="dr_sweet_advise" align="center">
<tr>
<td>
<div class="drword" id="drword" style="height:110px;"><#*** not_found_dict : 191***#> <#*** not_found_dict : 188***#>...
<br/>
<br/>
</div>
<div class="drImg"><img src="images/alertImg.png"></div>
<div style="height:70px;"></div>
</td>
</tr>
</table>
<!--[if lte IE 6.5]><iframe class="hackiframe"></iframe><![endif]-->
</div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" id="ruleForm" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get("productid"); %>">
<input type="hidden" name="current_page" value="Tools_Adbyby.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="6">
<input type="hidden" name="action_mode" value="toolscript">
<input type="hidden" name="action_script" value="/usr/sbin/k3c_adbyby.sh">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>">
<input type="hidden" name="adbyby_enable" value="<% nvram_get("adbyby_enable"); %>">
<input type="hidden" name="adbyby_video_mode" value="<% nvram_get("adbyby_video_mode"); %>">
<input type="hidden" name="adbyby_https_mode" value="<% nvram_get("adbyby_https_mode"); %>">
<input type="hidden" name="adbyby_monitor" value="<% nvram_get("adbyby_monitor"); %>">
<input type="hidden" name="save_name" value="">
<input type="hidden" name="save_content" value="">
<table class="content" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
<tr>
<td align="left" valign="top">
<table width="760px" border="0" cellpadding="5" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
<div class="formfonttitle">工具箱 - 广告屏蔽</div>
<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead><tr>
<td colspan="2" >广告屏蔽状态</td>
</tr></thead>
<tbody><tr >
<th width="30%" style="border-top: 0 none;">启用</th>
<td style="border-top: 0 none;">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_adbyby_enable"></div>
</td>
</tr>
<tr>
<th>广告屏蔽方式</th>
<td>
<input type="radio" name="adblock_mode" class="input" value="0" <% nvram_match("adblock_mode", "0", "checked"); %> onclick="hide_adb(this.value);">adbyby
<input type="radio" name="adblock_mode" class="input" value="1" <% nvram_match("adblock_mode", "1", "checked"); %> onclick="hide_adb(this.value);">koolproxy
</td>
</tr>
</tbody>
<thead><tr>
<td colspan="2" >高级配置</td>
</tr></thead>
<tr>
<th>运行模式</th>
<td>
<select name="adbyby_mode" class="input_option input_15_table">
<option value="0" <% nvram_match( "adbyby_mode", "0","selected"); %>>全局模式</option>
<option value="1" <% nvram_match( "adbyby_mode", "1","selected"); %>>广告列表模式</option>
</select>
</td>
</tr>
<tr >
<th width="30%" style="border-top: 0 none;">启用进程监控</th>
<td style="border-top: 0 none;">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_adbyby_monitor"></div>
</td>
</tr>
<tr id="kptr1">
<th width="30%" style="border-top: 0 none;">启用https过滤</th>
<td style="border-top: 0 none;">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_adbyby_https_mode"></div>
</td>
</tr>
<tr id="kptr2">
<th>Https 证书</th>
<td>
<input class="button_gen" onclick="location.href='/kool.crt'" type="button" value="下载证书" name="mycert" />
<a href="http://iytc.net/wordpress/?p=2046" target=_blank>【证书使用说明】</a>
</td>
</tr>
<tr id="kptr3">
<th width="30%" style="border-top: 0 none;">只过滤视频网站</th>
<td style="border-top: 0 none;">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_adbyby_video_mode"></div>
</td>
</tr>
<tr>
<th>不过滤的源IP<i>（留空表示过滤所有源IP）</i></th>
<td>
<textarea rows="4" cols="30" style="background-color: #475A5F;color:#FFFFFF;" name="adbyby_srcip" maxlength="100" placeholder="提示：每行一个IP地址"><% nvram_get("adbyby_srcip"); %></textarea>
</td>
</tr>
<tr>
<th>过滤https的源IP<i>（留空表示过滤所有源IP）</i></th>
<td>
<textarea rows="4" cols="30" style="background-color: #475A5F;color:#FFFFFF;" name="adbyby_httpsip" maxlength="100" placeholder="提示：每行一个IP地址"><% nvram_get("adbyby_httpsip"); %></textarea>
</td>
</tr>
</table>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="应用设置"/>
<input type="button" onClick="location.href=location.href" value="刷新状态" class="button_gen">
</div>
</td>
</tr>
</tbody>
</table>
</td>
</tr>
</table>
</td>
<td width="10" align="center" valign="top">&nbsp;</td>
</tr>
</table>
<div id="footer"></div>
</form>
</body>
</html>
