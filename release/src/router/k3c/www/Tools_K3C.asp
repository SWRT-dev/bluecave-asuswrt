<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache"/>
<meta HTTP-EQUIV="Expires" CONTENT="-1"/>
<link rel="shortcut icon" href="images/favicon.png"/>
<link rel="icon" href="images/favicon.png"/>
<title>软件中心 - K3C扩展设置</title>
<link rel="stylesheet" type="text/css" href="index_style.css"/>
<link rel="stylesheet" type="text/css" href="form_style.css"/>
<link rel="stylesheet" type="text/css" href="usp_style.css"/>
<link rel="stylesheet" type="text/css" href="ParentalControl.css">
<link rel="stylesheet" type="text/css" href="css/icon.css">
<link rel="stylesheet" type="text/css" href="css/element.css">
<link rel="stylesheet" type="text/css" href="/device-map/device-map.css">
<link rel="stylesheet" type="text/css" href="/calendar/fullcalendar.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/js/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" src="/calendar/jquery-ui.js"></script>
<style>

</style>
<script>
var k3c_disk='<% nvram_get("k3c_disk"); %>';

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
if(usbDevicesList[i].partition[j].partName==k3c_disk)
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
function init() {
	show_menu();
	show_partition();
}
function applyRule() {
if(document.getElementById("usb_disk_id").value==0)
{
alert("无法启用，请插入U盘或移动硬盘");
return;
}
document.form.submit();
}
function reload_Soft_Center(){
location.href = "/Softcenter.asp";
}


$(document).ready(function () {
$('#radio_k3c_enable').iphoneSwitch(document.form.k3c_enable.value,
function(){
document.form.k3c_enable.value = "1";
},
function(){
document.form.k3c_enable.value = "0";
}
);
});
</script>
</head>
<body onload="init();">
	<div id="TopBanner"></div>
	<div id="Loading" class="popup_bg"></div>
	<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
	<form method="post" name="form" id="ruleForm" action="/start_apply.htm" target="hidden_frame">
	<input type="hidden" name="current_page" value="Tools_K3C.asp"/>
	<input type="hidden" name="next_page" value="Tools_K3C.asp"/>
	<input type="hidden" name="group_id" value=""/>
	<input type="hidden" name="modified" value="0"/>
	<input type="hidden" name="action_mode" value="toolscript"/>
	<input type="hidden" name="action_script" value="k3c_tools.sh"/>
	<input type="hidden" name="action_wait" value=""/>
	<input type="hidden" name="first_time" value=""/>
	<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>"/>
	<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>"/>
	<input type="hidden" name="k3c_enable" value="<% nvram_get("k3c_enable"); %>">
	<input type="hidden" name="k3c_disk" value="<% nvram_get("k3c_disk"); %>">
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
							<table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3" class="FormTitle" id="FormTitle">
								<tr>
									<td bgcolor="#4D595D" colspan="3" valign="top">
										<div>&nbsp;</div>
										<div style="float:left;" class="formfonttitle">K3C扩展设置</div>
										<div style="float:right; width:15px; height:25px;margin-top:10px"><img id="return_btn" onclick="reload_Soft_Center();" align="right" style="cursor:pointer;position:absolute;margin-left:-30px;margin-top:-25px;" title="返回软件中心" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'"></img></div>
										<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
										<div class="formfontdesc" style="padding-top:5px;margin-top:0px;float: left;" id="cmdDesc">K3C扩展设置</div>
										<div style="color:#FC0;padding-top:5px;margin-top:25px;margin-left:0px;float: left;" id="NoteBox" >
                                                                                        <li style="margin-top:5px;">部分插件必须设置挂载U盘或移动硬盘 </li>
                                                                                        <li style="margin-top:5px;">剩余空间不得小于1G</li>
                                                                                        <li style="margin-top:5px;">其他待续</li>
										</div>
										<table style="margin:10px 0px 0px 0px;" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" id="routing_table">
											<thead>
											<tr>
												<td colspan="2">设置挂载分区</td>
											</tr>
											</thead>
											<tr >
											<th width="30%" style="border-top: 0 none;">启用</th>
											<td style="border-top: 0 none;">
											<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_k3c_enable"></div>
											</td>
											</tr>
<tr>
<th>选择要挂载的磁盘</th>
<td>
<select id="usb_disk_id" name="k3c_disk" class="input_option input_25_table">
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
<textarea cols="63" rows="20" wrap="off" readonly="readonly" id="textarea" style="width:99%;font-family:Courier New, Courier, mono; font-size:11px;background:#475A5F;color:#FFFFFF;">
<% nvram_dump("k3c.log",""); %>
</textarea>
</td></tr>
                                    	</table>
										<div class="apply_gen">
											<input class="button_gen" onclick="applyRule()" type="button" value="应用设置"/>
											<input type="button" onClick="location.href=location.href" value="刷新状态" class="button_gen">
										</div>
										<div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
										<div class="k3cBottom">
											K3C： <i>paldier</i><br/>
										</div>

									</td>
								</tr>
							</table>
						</td>
						<td width="10" align="center" valign="top"></td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
	</form>
	</td>
	<div id="footer"></div>
</body>
</html>
