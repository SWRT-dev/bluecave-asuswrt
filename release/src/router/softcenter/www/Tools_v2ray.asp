<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<title>软件中心 - v2ray</title>
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
#selectable .ui-selecting { background: #FECA40; }
#selectable .ui-selected { background: #F39814; color: white; }
#selectable .ui-unselected { background: gray; color: green; }
#selectable .ui-unselecting { background: green; color: black; }
#selectable { border-spacing:0px; margin-left:0px;margin-top:0px; padding: 0px; width:100%;}
#selectable td { height: 22px; }
.parental_th{
color:white;
background:#2F3A3E;
cursor: pointer;
width:160px;
height:22px;
border-bottom:solid 1px black;
border-right:solid 1px black;
}
.parental_th:hover{
background:rgb(94, 116, 124);
cursor: pointer;
}
.checked{
background-color:#9CB2BA;
width:82px;
border-bottom:solid 1px black;
border-right:solid 1px black;
}
.disabled{
width:82px;
border-bottom:solid 1px black;
border-right:solid 1px black;
}
#switch_menu{
text-align:right
}
#switch_menu span{
/*border:1px solid #222;*/
border-radius:4px;
font-size:16px;
padding:3px;
}
/*#switch_menu span:hover{
box-shadow:0px 0px 5px 3px white;
background-color:#97CBFF;
}*/
.click:hover{
box-shadow:0px 0px 5px 3px white;
background-color:#97CBFF;
}
.clicked{
background-color:#2894FF;
box-shadow:0px 0px 5px 3px white;
}
.click{
background:#8E8E8E;
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
<script>
function initial(){
show_menu();
document.form.v2ray_srcip.value = document.form.v2ray_srcip.value.replace(/,yushi,/g,"\r\n");
show_footer();
}
function applyRule() {
document.form.save_name.disabled=true;
document.form.save_content.disabled=true;
document.form.v2ray_srcip.value = document.form.v2ray_srcip.value.replace(/\r\n/g,",yushi,");
document.form.v2ray_srcip.value = document.form.v2ray_srcip.value.replace(/\n/g,",yushi,");
showLoading();
document.form.submit();
}
function reload_Soft_Center() {
	location.href = "/Softcenter.asp";
}
$(document).ready(function () {
$('#radio_v2ray_enable').iphoneSwitch(document.form.v2ray_enable.value,
function(){
document.form.v2ray_enable.value = "1";
},
function(){
document.form.v2ray_enable.value = "0";
}
);
});
$(document).ready(function () {
$('#radio_v2ray_udp_enable').iphoneSwitch(document.form.v2ray_udp_enable.value,
function(){
document.form.v2ray_udp_enable.value = "1";
},
function(){
document.form.v2ray_udp_enable.value = "0";
}
);
});
$(document).ready(function () {
$('#radio_v2ray_user').iphoneSwitch(document.form.v2ray_user.value,
function(){
document.form.v2ray_user.value = "1";
},
function(){
document.form.v2ray_user.value = "0";
}
);
});
</script></head>
<body onload="initial();" onunload="unload_body();" onselectstart="return false;">
<div id="TopBanner"></div>
<div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get("productid"); %>">
<input type="hidden" name="current_page" value="Tools_v2ray.asp">
<input type="hidden" name="next_page" value="">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_wait" value="2">
<input type="hidden" name="action_mode" value="toolscript">
<input type="hidden" name="action_script" value="softcenter_v2ray.sh">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>" disabled>
<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>">
<input type="hidden" name="v2ray_enable" value="<% nvram_get("v2ray_enable"); %>">
<input type="hidden" name="v2ray_udp_enable" value="<% nvram_get("v2ray_udp_enable"); %>">
<input type="hidden" name="ss_china_state" value="<% nvram_get("ss_china_state"); %>">
<input type="hidden" name="ss_foreign_state" value="<% nvram_get("ss_foreign_state"); %>">
<input type="hidden" name="v2ray_dns" value="<% nvram_get("v2ray_dns"); %>">
<input type="hidden" name="v2ray_user" value="<% nvram_get("v2ray_user"); %>">
<input type="hidden" name="save_name" value="">
<input type="hidden" name="save_content" value="">
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
<div id="content_title" class="formfonttitle" style="width:400px">工具箱 - v2ray</div>
            <div style="float:right; width:15px; height:25px;margin-top:10px">
             <img id="return_btn" onclick="reload_Soft_Center();" align="right" style="cursor:pointer;position:absolute;margin-left:-30px;margin-top:-25px;" title="返回软件中心" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'"></img>
            </div>
</td>
</tr>
</table>
<div style="margin:0px 0px 10px 5px;"><img src="/images/New_ui/export/line_export.png"></div>
</div>
<div id="PC_desc">
<table width="700px" style="margin-left:25px;">
<tr>
<td>
<div id="guest_image" style="background: url(images/New_ui/ssr.png);width: 100px;height: 87px;"></div>
</td>
<td>&nbsp;&nbsp;</td>
<td style="font-size: 14px;">
<span id="desc_title">使用步骤：</span>
<ol>
<li>首先打开扩展挂载并挂载虚拟内存</li>
<li>然后自行获取服务器参数</li>
<li>最后手动上传配置文件或在自定义配置里粘贴配置</li>
</ol>
<span id="desc_note" style="color:#FC0;">注意：</span>
<ol style="color:#FC0;margin:-5px 0px 3px -18px;*margin-left:18px;">
<li>测试版，不保证各项功能正常</li>
<li>所有别名及参数中不允许有">"字符。仅dns2socks模式socks5代理端口：23456 (待测) </li>
<li>本地代理必须为默认:协议socks端口1080</li>
</ol>
</td>
</tr>
</table>
</div>
<div id="edit_time_anchor"></div>
<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable">
<tr>
<th id="PC_enable">启用代理</th>
<td>
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_v2ray_enable"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_v2ray_enable').iphoneSwitch('<% nvram_get("v2ray_enable"); %>',
function(){
document.form.v2ray_enable.value = 1;
},
function(){
document.form.v2ray_enable.value = 0;
}
);
</script>
</div>
</td>
</tr>
</table>
<table id="list_table" width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
<tr>
<td valign="top" align="center">
<div id="VSList_Block"></div>
<div >
<table width="100%" border="1" cellspacing="0" cellpadding="4" class="FormTable">
<tr>
<th width="20%">UDP转发</th>
<td align="left">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_v2ray_udp_enable"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_v2ray_udp_enable').iphoneSwitch('<% nvram_get("v2ray_udp_enable"); %>',
function(){
document.form.v2ray_udp_enable.value = 1;
},
function(){
document.form.v2ray_udp_enable.value = 0;
}
);
</script>
</div>
</td>
</tr>
<tr>
<th>运行模式</th>
<td>
<select name="v2ray_mode" class="input_option input_15_table">
<option value="0" <% nvram_match( "v2ray_mode", "0","selected"); %>>国外代理模式</option>
<option value="1" <% nvram_match( "v2ray_mode", "1","selected"); %>>GFW列表模式</option>
<option value="2" <% nvram_match( "v2ray_mode", "2","selected"); %>>全局代理模式</option>
</select>
</td>
</tr>
<tr>
<th>DNS解析方式</th>
<td>
<select name="v2ray_dnsmode" class="input_option input_15_table">
<option value="0" <% nvram_match( "v2ray_dnsmode", "0","selected"); %>>远程解析模式</option>
<option value="1" <% nvram_match( "v2ray_dnsmode", "1","selected"); %>>Pdnsd解析模式</option>
<option value="2" <% nvram_match( "v2ray_dnsmode", "2","selected"); %>>dns2socks模式</option>
</select>
<a href="http://www.ip111.cn/" target=_blank>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;【 分流检测 】</a>
</td>
</tr>
<tr>
<th>国外DNS</th>
<td>
<select name="v2ray_dns" class="input_option input_15_table">
<option value="0" <% nvram_match( "v2ray_dns", "0","selected"); %>>opendns</option>
<option value="1" <% nvram_match( "v2ray_dns", "1","selected"); %>>googledns</option>
</select>
</td>
</tr>
<tr>
<th width="20%">启用v2ray配置</th>
<td align="left">
<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_v2ray_user"></div>
<div class="iphone_switch_container" style="height:32px; width:74px; position: relative; overflow: hidden">
<script type="text/javascript">
$('#radio_v2ray_user').iphoneSwitch('<% nvram_get("v2ray_user"); %>',
function(){
document.form.v2ray_user.value = 1;
},
function(){
document.form.v2ray_user.value = 0;
}
);
</script>
</div>
</td>
</tr>
<tr>
<th>自定义配置</th>
<td>
<textarea rows="6" cols="44" style="width:99%;background-color: #475A5F;color:#FFFFFF;" name="v2ray_srcip" maxlength="5000" placeholder="提示： 手动输入自定义配置，正常运行后请关闭启用v2ray配置选项，使用备份配置运行，不然每重启均保存一次配置。"><% nvram_show_chinese_char("v2ray_srcip"); %></textarea>
</td>
</tr>
</table>
</div>
<div id="ss_status">
<table style="margin:-1px 0px 0px 0px;" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" >
<tr id="ss_state">
<th id="mode_state" width="35%">运行状态</th>
<td>
<div style="display:table-cell;float: left;margin-left:0px;">
<span id="ss_state1"><% nvram_get("ss_foreign_state"); %></span>
<br/>
<span id="ss_state2"><% nvram_get("ss_china_state"); %></span>
</div>
</td>
</tr>
<thead>
<tr>
<td colspan="2">运行信息</td>
</tr>
</thead>
<tr><td colspan="2">
<textarea cols="63" rows="25" wrap="off" readonly="readonly" id="textarea" style="width:99%;font-family:Courier New, Courier, mono; font-size:11px;background:#475A5F;color:#FFFFFF;">
<% nvram_dump("v2ray.log",""); %>
</textarea>
</td></tr>
</table>
</div>
<div class="apply_gen">
<input class="button_gen" onclick="applyRule()" type="button" value="应用设置"/>
<input type="button" onClick="location.href=location.href" value="刷新状态" class="button_gen">
</div>
</td>
</tr>
</table>
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
