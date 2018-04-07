<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title id="web_title"><#587#> - 迅雷远程</title>
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
var xunlei_disk='<% nvram_get("xunlei_disk"); %>';
var partitions_array = [];
function show_partition(){
require(['/require/modules/diskList.js?hash=' + Math.random().toString()], function(diskList){
var code="";
var mounted_partition = 0;
partitions_array = [];
var usbDevicesList = diskList.list();
for(var i=0; i < usbDevicesList.length; i++){
for(var j=0; j < usbDevicesList[i].partition.length; j++){
partitions_array.push(usbDevicesList[i].partition[j].mountPoint);
var accessableSize = simpleNum(usbDevicesList[i].partition[j].size-usbDevicesList[i].partition[j].used);
var totalSize = simpleNum(usbDevicesList[i].partition[j].size);
if(usbDevicesList[i].partition[j].status == "unmounted")
continue;
if(usbDevicesList[i].partition[j].partName==xunlei_disk)
code +='<option value="'+ usbDevicesList[i].partition[j].partName+'" selected="selected">'+ usbDevicesList[i].partition[j].partName+'(空闲:'+accessableSize+' GB)</option>';
else
code +='<option value="'+ usbDevicesList[i].partition[j].partName+'" >'+ usbDevicesList[i].partition[j].partName+'(空闲:'+accessableSize+' GB)</option>';
mounted_partition++;
}
}
if(mounted_partition == 0)
code +='<option value="0">无U盘或移动硬盘</option>';
document.getElementById("usb_disk_id").innerHTML = code;
});
}
function initial(){
show_menu();
show_footer();
show_partition()
}
function reactive(){
document.form.xunlei_react.value = "1";
}
function applyRule(_on){
if(document.getElementById("usb_disk_id").value==0)
{
alert("无法启用，请插入U盘或移动硬盘");
return;
}
showLoading();
document.form.submit();
}
</script></head>
<body onload="initial();" onunload="unload_body();" onselectstart="return false;">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get("productid"); %>">
<input type="hidden" name="current_page" value="Tools_xunlei.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="action_mode" value="toolscript">
<input type="hidden" name="action_script" value="/usr/sbin/k3c_xunlei.sh">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>" disabled>
<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>">
<input type="hidden" name="xunlei_enable" value="<% nvram_get("xunlei_enable"); %>">
<input type="hidden" name="xunlei_react" value="0">
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
<div id="content_title" class="formfonttitle" style="width:400px">迅雷远程服务</div>
</td>
</tr>
</table>
<div style="margin:0px 0px 10px 5px;"><img src="/images/New_ui/export/line_export.png"></div>
</div>
<div id="PC_desc">
<table width="700px" style="margin-left:25px;">
<tr>
<td>
<div id="guest_image" style="background: url(images/New_ui/xunlei.png);width: 100px;height: 87px;"></div>
</td>
<td>&nbsp;&nbsp;</td>
<td style="font-style: italic;font-size: 14px;">
<span id="desc_title">使用简介：</span>
<ol>
<li>迅雷远程服务可以在远程控制设备进行下载</li>
<li>你需要插入U盘或移动硬盘才能启动此功能</li>
<li>第一次启动时会生成激活码，你需要在【<a href="http://yuancheng.xunlei.com" target=_bllank style="text-decoration:underline;">迅雷远程网站</a>】注册并绑定激活码</li>
</ol>
<span id="desc_note" style="color:#FC0;">提示：</span>
<ol style="color:#FC0;margin:-5px 0px 3px -18px;*margin-left:18px;">
<li>所接入的U盘或硬盘必须大于8G</li> </ol>
</td>
</tr>
</table>
</div>
<div id="edit_time_anchor"></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead><tr>
<td colspan="2" >服务信息</td>
</tr></thead>
<tr>
<th id="PC_enable">启用迅雷远程</th>
<td>
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_xunlei_enable"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_xunlei_enable').iphoneSwitch('<% nvram_get("xunlei_enable"); %>',
function(){
document.form.xunlei_enable.value = 1;
},
function(){
document.form.xunlei_enable.value = 0;
}
);
</script>
</div>
</td>
</tr>
<tr>
<th>设备激活码</th>
<td id="xunlei_status">
<% nvram_get("xunlei_num"); %>
<input class="button_gen" onclick="reactive();applyRule()" type="button" value="重新生成激活码"/>
</td>
</tr>
<tr>
<th>选择程序运行的磁盘</th>
<td>
<select id="usb_disk_id" name="xunlei_disk" class="input_option input_25_table">
<option value="0">无U盘或移动硬盘</option>
</select>
</td>
</tr>
<thead>
<tr>
<td colspan="2">日志信息</td>
</tr>
</thead>
<tr><td colspan="2">
<textarea cols="63" rows="25" wrap="off" readonly="readonly" id="textarea" style="width:99%;font-family:Courier New, Courier, mono; font-size:11px;background:#475A5F;color:#FFFFFF;">
<% nvram_dump("xunlei.log",""); %>
</textarea>
</td></tr>
</table>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="应用设置"/>
<input type="button" onClick="location.href=location.href" value="刷新日志" class="button_gen">
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
