<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title id="web_title"><#587#> - 内网穿透</title>
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<link rel="stylesheet" type="text/css" href="ParentalControl.css">
<link rel="stylesheet" type="text/css" href="index_style.css">
<link rel="stylesheet" type="text/css" href="form_style.css">
<link rel="stylesheet" type="text/css" href="usp_style.css">
<link rel="stylesheet" type="text/css" href="/calendar/fullcalendar.css">
<link rel="stylesheet" type="text/css" href="/device-map/device-map.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/js/jquery.js"></script>
<script type="text/javascript" src="/calendar/jquery-ui.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<style>
</style>
<script>
var vts_rulelist_array = "<% nvram_char_to_ascii("","ngrok_rulelist"); %>";
function validForm(){
if(!Block_chars(document.form.vts_desc_x_0, ["<" ,">" ,"'" ,"%"])){
return false;
}
if(document.form.vts_lport_x_0.value.length > 0
&& !validator.numberRange(document.form.vts_lport_x_0, 1, 65535)){
return false;
}
if(document.form.vts_desc_x_0.value==""){
alert("子域名不能为空");
document.form.vts_ipaddr_x_0.focus();
document.form.vts_ipaddr_x_0.select();
return false;
}
if(document.form.vts_ipaddr_x_0.value==""){
alert("IP地址不能为空");
document.form.vts_ipaddr_x_0.focus();
document.form.vts_ipaddr_x_0.select();
return false;
}
if(document.form.vts_lport_x_0.value==""){
alert("本地端口不能为空");
document.form.vts_lport_x_0.focus();
document.form.vts_lport_x_0.select();
return false;
}
return true;
}
var overlib_str = new Array();
function showvts_rulelist(){
var vts_rulelist_row = decodeURIComponent(vts_rulelist_array).split('<');
var code = "";
code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="vts_rulelist_table">';
if(vts_rulelist_row.length == 1)
code +='<tr><td style="color:#FFCC00;" colspan="6">没有配置数据</td></tr>';
else{
for(var i = 1; i < vts_rulelist_row.length; i++){
overlib_str[i] ="";
code +='<tr id="row'+i+'">';
var vts_rulelist_col = vts_rulelist_row[i].split('>');
var wid=[13, 27, 21, 12, 12];
for(var j = 0; j < vts_rulelist_col.length; j++){
if(j !=1){
code +='<td width="'+wid[j]+'%">'+ vts_rulelist_col[j] +'</td>';
}else {
if(vts_rulelist_col[1].length >23){
overlib_str[i] += vts_rulelist_col[1];
vts_rulelist_col[1]=vts_rulelist_col[1].substring(0, 21)+"...";
code +='<td width="'+wid[j]+'%" title='+overlib_str[i]+'>'+ vts_rulelist_col[1] +'</td>';
}else
code +='<td width="'+wid[j]+'%">'+ vts_rulelist_col[j] +'</td>';
}
}
code +='<td width="15%"><input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
}
}
code +='</table>';
document.getElementById("vts_rulelist_Block").innerHTML = code;
}
function addRow(obj, head){
if(head == 1)
vts_rulelist_array += "<"
else
vts_rulelist_array += ">"
vts_rulelist_array += obj.value;
obj.value = "";
}
function addRow_Group(upper){
if(validForm()){
var rule_num = document.getElementById('vts_rulelist_table').rows.length;
var item_num = document.getElementById('vts_rulelist_table').rows[0].cells.length;
if(rule_num >= upper){
alert("超过数目限制：" + upper + "条");
return false;
}
addRow(document.form.vts_proto_x_0, 1);
addRow(document.form.vts_desc_x_0 ,0);
addRow(document.form.vts_ipaddr_x_0, 0);
addRow(document.form.vts_lport_x_0, 0);
addRow(document.form.vts_rport_x_0, 0);
document.form.vts_proto_x_0.value="TCP";
showvts_rulelist();
return true;
}
}
function del_Row(r){
var i=r.parentNode.parentNode.rowIndex;
document.getElementById('vts_rulelist_table').deleteRow(i);
var vts_rulelist_value = "";
for(k=0; k<document.getElementById('vts_rulelist_table').rows.length; k++){
for(j=0; j<document.getElementById('vts_rulelist_table').rows[k].cells.length-1; j++){
if(j == 0)
vts_rulelist_value += "<";
else
vts_rulelist_value += ">";
if(document.getElementById('vts_rulelist_table').rows[k].cells[j].innerHTML.lastIndexOf("...")<0){
vts_rulelist_value += document.getElementById('vts_rulelist_table').rows[k].cells[j].innerHTML;
}else{
vts_rulelist_value += document.getElementById('vts_rulelist_table').rows[k].cells[j].title;
}
}
}
vts_rulelist_array = vts_rulelist_value;
if(vts_rulelist_array == "")
showvts_rulelist();
}
function initial(){
show_menu();
show_footer();
document.getElementById("ngrok_server").value= '<% nvram_get("ngrok_server"); %>';
document.getElementById("ngrok_port").value= '<% nvram_get("ngrok_port"); %>';
document.getElementById("ngrok_token").value= '<% nvram_get("ngrok_token"); %>';
showvts_rulelist();
}
function applyRule(_on){
var rule_num = document.getElementById('vts_rulelist_table').rows.length;
var item_num = document.getElementById('vts_rulelist_table').rows[0].cells.length;
var tmp_value = "";
for(i=0; i<rule_num; i++){
tmp_value += "<"
for(j=0; j<item_num-1; j++){
if(document.getElementById('vts_rulelist_table').rows[i].cells[j].innerHTML.lastIndexOf("...")<0){
tmp_value += document.getElementById('vts_rulelist_table').rows[i].cells[j].innerHTML;
}else{
tmp_value += document.getElementById('vts_rulelist_table').rows[i].cells[j].title;
}
if(j != item_num-2)
tmp_value += ">";
}
}
if(tmp_value == "<"+"没有配置数据" || tmp_value == "<")
tmp_value = "";
document.form.ngrok_rulelist.value = tmp_value;
showLoading();
document.form.submit();
}
function reload_Soft_Center() {
	location.href = "/Softcenter.asp";
}
</script></head>
<body onload="initial();" onunload="unload_body();" onselectstart="return false;">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get("productid"); %>">
<input type="hidden" name="current_page" value="Tools_ngrok.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="action_mode" value="toolscript">
<input type="hidden" name="action_script" value="/usr/sbin/k3c_ngrok.sh">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>" disabled>
<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>">
<input type="hidden" name="ngrok_enable" value="<% nvram_get("ngrok_enable"); %>">
<input type="hidden" name="ngrok_server" value="<% nvram_get("ngrok_server"); %>">
<input type="hidden" name="ngrok_port" value="<% nvram_get("ngrok_port"); %>">
<input type="hidden" name="ngrok_token" value="<% nvram_get("ngrok_token"); %>">
<input type="hidden" name="ngrok_id" value="<% nvram_get("ngrok_id"); %>">
<input type="hidden" name="ngrok_rulelist" value=''>
<table class="content" align="center" cellpadding="0" cellspacing="0" >
<tr>
<td width="17">&nbsp;</td>
<td valign="top" width="202">
<div id="mainMenu"></div>
<div id="subMenu"></div>
</td>
<td valign="top">
<div id="tabMenu" class="submenuBlock"></div>
<table width="98%" border="0" align="left" cellpadding="0" cellspacing="0" >
<tr>
<td valign="top" >
<table width="730px" border="0" cellpadding="4" cellspacing="0" class="FormTitle" id="FormTitle">
<tbody>
<tr>
<td bgcolor="#4D595D" valign="top">
<div>&nbsp;</div>
<div style="margin-top:-5px;">
<table width="730px">
<tr>
<td align="left" >
<div id="content_title" class="formfonttitle" style="width:400px">Ngrok内网穿透服务</div>
            <div style="float:right; width:15px; height:25px;margin-top:10px">
             <img id="return_btn" onclick="reload_Soft_Center();" align="right" style="cursor:pointer;position:absolute;margin-left:-30px;margin-top:-25px;" title="返回软件中心" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'"></img>
            </div>
</td>
</tr>
</table>
<div style="margin:0px 0px 10px 5px;"><img src="/images/New_ui/export/line_export.png"></div>
</div>
<div id="edit_time_anchor"></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead><tr>
<td colspan="2" >服务器选项</td>
</tr></thead>
<tr>
<th id="PC_enable">启用Ngrok</th>
<td>
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_ngrok_enable"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_ngrok_enable').iphoneSwitch('<% nvram_get("ngrok_enable"); %>',
function(){
document.form.ngrok_enable.value = 1;
},
function(){
document.form.ngrok_enable.value = 0;
}
);
</script>
</div>
</td>
</tr>
<tr>
<th width="20%">服务器名称</th>
<td align="left">
<input maxlength="25" class="input_25_table" name="ngrok_server" id="ngrok_server" autocorrect="off" autocapitalize="off" type="text">
</td>
</tr>
<tr>
<th width="20%">服务器端口</th>
<td align="left">
<input maxlength="25" class="input_25_table" name="ngrok_port" id="ngrok_port" autocorrect="off" autocapitalize="off" type="text">
</td>
</tr>
<tr>
<th width="20%">Token认证串</th>
<td align="left">
<input maxlength="40" class="input_32_table" name="ngrok_token" id="ngrok_token" autocorrect="off" autocapitalize="off" type="text">
</td>
</tr>
</table>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table">
<thead>
<tr>
<td colspan="7">隧道列表&nbsp;(<#1484#>&nbsp;32)</td>
</tr>
</thead>
<tr>
<th>协议类型</th>
<th>子域名</th>
<th>本地IP地址</th>
<th>本地端口</th>
<th>远程端口</th>
<th>增加/删除</th>
</tr>
<tr>
<td width="13%">
<select name="vts_proto_x_0" class="input_option">
<option value="http">HTTP</option>
<option value="tcp">TCP</option>
<option value="https">HTTPS</option>
</select>
</td>
<td width="27%">
<input type="text" maxlength="30" class="input_20_table" name="vts_desc_x_0" onKeyPress="return validator.isString(this, event)" autocorrect="off" autocapitalize="off"/>
</td>
<td width="21%">
<input type="text" maxlength="15" class="input_15_table" name="vts_ipaddr_x_0" align="left" onkeypress="return validator.isIPAddr(this, event)" style="float:left;"/ autocomplete="off" autocorrect="off" autocapitalize="off">
<div id="ClientList_Block" class="clientlist_dropdown" style="margin-left:2px;margin-top:25px;"></div>
</td>
<td width="12%">
<input type="text" maxlength="5" class="input_6_table" name="vts_lport_x_0" onkeypress="return validator.isPortRange(this, event)" autocorrect="off" autocapitalize="off"/>
</td>
<td width="12%">
<input type="text" maxlength="5" class="input_6_table" name="vts_rport_x_0" onkeypress="return validator.isPortRange(this, event)" autocorrect="off" autocapitalize="off"/>
</td>
<td width="15%">
<input type="button" class="add_btn" onClick="addRow_Group(32);" name="vts_rulelist2" value="">
</td>
</tr>
</table>
<div id="vts_rulelist_Block"></div>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="应用设置"/>
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
