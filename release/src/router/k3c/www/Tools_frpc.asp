<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title><#587#> - frpc</title>
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
function initial(){
show_menu();
show_footer();
}
function applyRule(_on){
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
<input type="hidden" name="current_page" value="Tools_frpc.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="">
<input type="hidden" name="action_mode" value="toolscript">
<input type="hidden" name="action_script" value="/usr/sbin/k3c_frpc.sh">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>" disabled>
<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>">
<input type="hidden" name="frpc_enable" value="<% nvram_get("frpc_enable"); %>">
<input type="hidden" name="frpc_auto" value="<% nvram_get("frpc_auto"); %>">
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
<div id="content_title" class="formfonttitle" style="width:400px">工具箱 - frpc穿透</div>
</td>
</tr>
</table>
<div style="margin:0px 0px 10px 5px;"><img src="/images/New_ui/export/line_export.png"></div>
</div>
<div id="PC_desc">
<table width="700px" style="margin-left:25px;">
<tr>
<td>
<div id="guest_image" style="background: url(images/New_ui/frpc.png);width: 60px;height: 60px;"></div>
</td>
<td>&nbsp;&nbsp;</td>
<td style="font-size: 14px;">
<span id="desc_title">frpc简介：</span>
<ol>
<li>穿透服务用于内网穿透</li>
<li>重启选项 每天自动重启</li>
<li>需要修改 /jffs/frp/frpc.ini 文件相应设置</li>
</ol>
<span id="desc_note" style="color:#FC0;">提示：</span>
<ol style="color:#FC0;margin:-5px 0px 3px -18px;*margin-left:18px;">
<li>重启选项 默认重启，打开选项开关禁用自动重启</li>
</ol>
</td>
</tr>
</table>
</div>
<div id="edit_time_anchor"></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<thead><tr>
<td colspan="2" >控制选项</td>
</tr></thead>
<tr>
<th id="PC_enable">启用穿透</th>
<td>
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_frpc_enable"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_frpc_enable').iphoneSwitch('<% nvram_get("frpc_enable"); %>',
function(){
document.form.frpc_enable.value = 1;
},
function(){
document.form.frpc_enable.value = 0;
}
);
</script>
</div>
</td>
</tr>
<tr>
<th width="20%">禁用自动重启</th>
<td align="left">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_frpc_auto"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_frpc_auto').iphoneSwitch('<% nvram_get("frpc_auto"); %>',
function(){
document.form.frpc_auto.value = 1;
},
function(){
document.form.frpc_auto.value = 0;
}
);
</script>
</div>
</td>
</tr>
<thead>
<tr>
<td colspan="2">日志信息</td>
</tr>
</thead>
<tr><td colspan="2">
<textarea cols="63" rows="25" wrap="off" readonly="readonly" id="textarea" style="width:99%;font-family:Courier New, Courier, mono; font-size:11px;background:#475A5F;color:#FFFFFF;">
<% nvram_dump("frpc.log",""); %>
</textarea>
</td></tr>
</table>
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

