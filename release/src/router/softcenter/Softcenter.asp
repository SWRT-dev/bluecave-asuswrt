<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache"/>
<meta HTTP-EQUIV="Expires" CONTENT="-1"/>
<link rel="shortcut icon" href="images/favicon.png"/>
<link rel="icon" href="images/favicon.png"/>
<title sclang>jffs extended settings</title>
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
<script type="text/javascript" src="/js/i18n.js"></script>
<style>

</style>
<script>
var sc_disk='<% nvram_get("sc_disk"); %>';

var partitions_array = [];
function show_partition(){
require(['/require/modules/diskList.js?hash=' + Math.random().toString()], function(diskList){
var code="";
var free=dict['Free'];
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
if(usbDevicesList[i].partition[j].partName==sc_disk)
code +='<option value="'+ usbDevicesList[i].partition[j].partName+'" selected="selected">'+ usbDevicesList[i].partition[j].partName+'(' + free +':'+accessableSize+' GB)</option>';
else
code +='<option value="'+ usbDevicesList[i].partition[j].partName+'" >'+ usbDevicesList[i].partition[j].partName+'(' + free +':'+accessableSize+' GB)</option>';
mounted_partition++;
}
}
if(mounted_partition == 0)
code +='<option value="0" sclang>No Disk</option>';
document.getElementById("usb_disk_id").innerHTML = code;
});
}
function init() {
	show_menu();
	sc_load_lang("sc");
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
$('#radio_sc_mount').iphoneSwitch(document.form.sc_mount.value,
function(){
document.form.sc_mount.value = "1";
},
function(){
document.form.sc_mount.value = "0";
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
	<input type="hidden" name="current_page" value="Softcenter.asp"/>
	<input type="hidden" name="next_page" value="Softcenter.asp"/>
	<input type="hidden" name="group_id" value=""/>
	<input type="hidden" name="modified" value="0"/>
	<input type="hidden" name="action_mode" value="toolscript"/>
	<input type="hidden" name="action_script" value="jffsinit.sh"/>
	<input type="hidden" name="action_wait" value=""/>
	<input type="hidden" name="first_time" value=""/>
	<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>"/>
	<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>"/>
	<input type="hidden" name="sc_mount" value="<% nvram_get("sc_mount"); %>">
	<input type="hidden" name="sc_disk" value="<% nvram_get("sc_disk"); %>">
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
										<div style="float:left;" class="formfonttitle" sclang>jffs extended settings</div>
										<div style="margin:30px 0 10px 5px;" class="splitLine"></div>
										<div class="formfontdesc" style="padding-top:5px;margin-top:0px;float: left;" id="cmdDesc" sclang>jffs extended settings</div>
										<div style="color:#FC0;padding-top:5px;margin-top:25px;margin-left:0px;float: left;" id="NoteBox" >
                                                                                        <li style="margin-top:5px;" sclang>This function must be enabled when JFFS is less than 50MB.</li>
                                                                                        <li style="margin-top:5px;" sclang>Support EXT/FAT/NTFS partitions(BLUECAVE unsupport FAT).</li>
                                                                                        <li style="margin-top:5px;" sclang>No less than 1GB of free space.</li>
                                                                                        <li style="margin-top:5px;" sclang>Must unmount the current partition before mounting other partitions.</li>
										</div>
										<table style="margin:10px 0px 0px 0px;" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" id="routing_table">
											<thead>
											<tr>
												<td colspan="2" sclang>Setting</td>
											</tr>
											</thead>
											<tr >
											<th width="30%" style="border-top: 0 none;" sclang>Enable</th>
											<td style="border-top: 0 none;">
											<div align="center" class="left" style="width:94px; float:left; cursor:pointer;" id="radio_sc_mount"></div>
											</td>
											</tr>
<tr>
<th sclang>Select a partition to mount</th>
<td>
<select id="usb_disk_id" name="sc_disk" class="input_option input_25_table">
<option value="0" sclang>No Disk</option>
</select>
</td>
</tr>

                                    	</table>
										<div class="apply_gen">
											<input class="button_gen" onclick="applyRule()" type="button" value="Apply"/ sclang>
											<input type="button" onClick="location.href=location.href" value="Refresh" class="button_gen" sclang>
										</div>
										<div style="margin:30px 0 10px 5px;" class="splitLine"></div>
										<div class="scBottom">
											webui by： <i>paldier</i><br/>
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
