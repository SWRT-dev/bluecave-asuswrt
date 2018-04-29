<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
	<meta http-equiv="X-UA-Compatible" content="IE=Edge" />
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
	<meta HTTP-EQUIV="Pragma" CONTENT="no-cache" />
	<meta HTTP-EQUIV="Expires" CONTENT="-1" />
	<link rel="shortcut icon" href="images/favicon.png" />
	<link rel="icon" href="images/favicon.png" />
	<title>软件中心 - V2ray</title>
	<link rel="stylesheet" type="text/css" href="index_style.css" />
	<link rel="stylesheet" type="text/css" href="form_style.css" />
	<link rel="stylesheet" type="text/css" href="usp_style.css" />
	<link rel="stylesheet" type="text/css" href="ParentalControl.css">
	<link rel="stylesheet" type="text/css" href="css/icon.css">
	<link rel="stylesheet" type="text/css" href="css/element.css">
	<script type="text/javascript" src="/state.js"></script>
	<script type="text/javascript" src="/popup.js"></script>
	<script type="text/javascript" src="/help.js"></script>
	<script type="text/javascript" src="/validator.js"></script>
	<script type="text/javascript" src="/js/jquery.js"></script>
	<script type="text/javascript" src="/general.js"></script>
	<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
	<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
	<script type="text/javascript" src="/dbconf?p=v2ray_&v=<% uptime(); %>"></script>
	<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
	<script>
		var $j = jQuery.noConflict();
		function init() {
			show_menu(menu_hook);
			buildswitch();
			//version_show();
			var rrt = document.getElementById("switch");
			if (document.form.v2ray_enable.value != "1") {
				rrt.checked = false;
			} else {
				rrt.checked = true;
			}

			//用于显示和隐藏path输入框和隐藏类型选择框
			$j("#v2ray_network").change(function () {
				var networkData = $j(this).val();
				if (networkData == "ws") {
					$j("#tr_v2ray_headtype").hide();
					$j("#tr_v2ray_network_path").fadeIn();
					$j("#tr_v2ray_network_host").hide();
				}else if(networkData == "ws_hd"){
					$j("#tr_v2ray_headtype").hide();
					$j("#tr_v2ray_network_path").fadeIn();
					$j("#tr_v2ray_network_host").fadeIn();
				}else {
					$j("#tr_v2ray_network_path").hide();
					$j("#tr_v2ray_network_host").hide();
					setHeadType(networkData);
					$j("#tr_v2ray_headtype").fadeIn();
				}
			});
			$j("#v2ray_server_address").val(db_v2ray_["v2ray_server_address"]);//服务端地址
			$j("#v2ray_server_port").val(db_v2ray_["v2ray_server_port"]);//服务端端口
			$j("#v2ray_id").val(db_v2ray_["v2ray_id"]);//用户ID
			$j("#v2ray_alterid").val(db_v2ray_["v2ray_alterid"]);//额外ID
			$j("#v2ray_security").val(db_v2ray_["v2ray_security"]);//加密方式
			$j("#v2ray_network").val(db_v2ray_["v2ray_network"]).change();//传输协议
			$j("#v2ray_headtype").val(db_v2ray_["v2ray_headtype"]);//伪装类型
			$j("#v2ray_network_path").val(db_v2ray_["v2ray_network_path"]);//伪装路径
			$j("#v2ray_network_host").val(db_v2ray_["v2ray_network_host"]);//伪装headers
			$j("#v2ray_network_security").val(db_v2ray_["v2ray_network_security"]);//底层加密方式
			$j("#v2ray_network_mux").val(db_v2ray_["v2ray_network_mux"]);//多路复用mux
			//显示日志框
			$j("#v2ray_log_button").click(function () {
				get_Log();
				$j("#vpnc_settings").fadeIn();
			});
			//隐藏日志框
			$j("#close_log_win").click(function () {
				$j("#vpnc_settings").fadeOut();
			});
			//获取状态
			get_satus();
		}
		function done_validating() {
			return true;
		}

		function buildswitch() {
			$j("#switch").click(
				function () {
					if (document.getElementById('switch').checked) {
						document.form.v2ray_enable.value = 1;
					} else {
						document.form.v2ray_enable.value = 0;
					}
				});
		}

		function onSubmitCtrl(o, s) {
			if (validatorDone()) {
				document.form.action_mode.value = s;
				showLoading(10);
				document.form.submit();
				setTimeout(function () { get_satus(); }, 10100);
			}
		}

		function reload_Soft_Center() {
			location.href = "/Main_Soft_center.asp";
		}
		//数据简单校验
		function validatorDone() {
			if (!isNumber($j("#v2ray_server_port").val())) {
				alert("你家端口居然不是数字??");
				$j("#v2ray_server_port").focus();
				return false;
			}

			if (!isNumber($j("#v2ray_alterid").val())) {
				alert("额外id必须是数字");
				$j("#v2ray_alterid").focus();
				return false;
			}

			if (!checkUUID($j("#v2ray_id").val())) {
				alert("用户ID出错,必须是uuid");
				$j("#v2ray_id").focus();
				return false;
			}
			return true;
		}

		//UUID校验
		function checkUUID(value) {
			var reg = /^[0-9a-z]{8}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{12}$/;
			if (value.match(reg) == null || value == "") {
				return false;
			} else {
				return true;
			}
		}
		//数字校验
		function isNumber(value) {
			var reg = /^(-)?\d+(\.\d+)?$/;
			if (value.match(reg) == null || value == "") {
				return false
			} else {
				return true
			}
		}
		//设置伪装类型选择
		function setHeadType(type) {
			var headType = getHeadTypeData(type);
			$j("#v2ray_headtype").empty();
			if (headType) {
				$j.each(headType, function (key, val) {
					$j("#v2ray_headtype").append('<option value="' + val + '">' + key + '</option>');
				});
			}
		}
		//获取伪装类型
		function getHeadTypeData(type) {
			switch (type) {
				case "tcp":
					return { "不伪装": "none", "伪装http": "http" };
					break;
				case "kcp":
					return { "不伪装": "none", "伪装视频通话(srtp)": "srtp", "伪装BT下载(uTP)": "utp", "伪装微信视频通话": "wechat-video" };
					break;
				default:
					return null;
					break;
			}
		}
		function upload_v2ray_backup() {
			if ($j('#v2ray_file').val == "") return false;
			filename="v2ray_conf_backup.txt";
			document.form.enctype = "multipart/form-data";
			document.form.encoding = "multipart/form-data";
			document.form.action = "/ssupload.cgi?a=/tmp/" + filename;
			document.form.submit();
			$j('#v2ray_file_info').text("完成");
		}
		//执行替换配置操作
		function replace_v2ray_config(){
			$j.ajax({
				url: 'apply.cgi?current_page=Module_v2ray.asp&next_page=Module_v2ray.asp&group_id=&modified=0&action_mode=+Refresh+&action_script=&action_wait=&first_time=&preferred_lang=CN&SystemCmd=v2ray_conf_replace.sh&firmver=3.0.0.4',
				contentType: "application/x-www-form-urlencoded",
				dataType: 'text',
				error: function (xhr) {
					alert("error");
				},
				success: function(response) {
					showLoading(10);
				    setTimeout(function () { get_satus(); }, 10100);
				}
            });
		}
		//执行重启shell
		function restart_v2ray(){
			$j.ajax({
				url: 'apply.cgi?current_page=Module_v2ray.asp&next_page=Module_v2ray.asp&group_id=&modified=0&action_mode=+Refresh+&action_script=&action_wait=&first_time=&preferred_lang=CN&SystemCmd=v2ray_restart.sh&firmver=3.0.0.4',
				contentType: "application/x-www-form-urlencoded",
				dataType: 'text',
				error: function (xhr) {
					alert("error");
				},
				success: function(response) {
					showLoading(10);
				    setTimeout(function () { get_satus(); }, 10100);
				}
            });
		}
		//执行更新shell
		function update_v2ray(){
			$j.ajax({
				url: 'apply.cgi?current_page=Module_v2ray.asp&next_page=Module_v2ray.asp&group_id=&modified=0&action_mode=+Refresh+&action_script=&action_wait=&first_time=&preferred_lang=CN&SystemCmd=v2ray_update.sh&firmver=3.0.0.4',
				contentType: "application/x-www-form-urlencoded",
				dataType: 'text',
				error: function (xhr) {
					alert("error");
				},
				success: function(response) {
					showLoading(10);
				    setTimeout(function () { get_satus(); }, 10100);
				}
            });
		}
		//获取状态
		function get_satus() {
			$j.ajax({
				url: 'apply.cgi?current_page=Module_v2ray.asp&next_page=Module_v2ray.asp&group_id=&modified=0&action_mode=+Refresh+&action_script=&action_wait=&first_time=&preferred_lang=CN&SystemCmd=v2ray_status.sh&firmver=3.0.0.4',
				dataType: 'html',
				error: function (xhr) {
					alert("error");
				},
				success: function (response) {
					setTimeout(function () { get_satus_log(); }, 1000);
				}
			});
		}
		var satusnum = 0;
		//加载状态
		function get_satus_log() {
			$j("#v2ray_status").empty();
			$j("#v2ray_status").append("<span>获取状态中.....</span>");
			$j.get("/res/v2ray_status.htm", function (data, status) {
				if (status == "error") {
					$j("#v2ray_status").append("<span style='color:red;'>获取状态错误!!!!!</span>");
					console.log(status);
					return;
				}
				var obj = isJsonString(data);
				if (!obj) {
					if (satusnum < 10) {
						setTimeout(function () { get_satus_log(); }, 1000);
						satusnum++;
					}
					return;
				}
				$j("#v2ray_status").empty();
				
				if (obj.is_run) {
					$j("#v2ray_status").append("<p style='color:green;'>V2ray运行中...</p>");
				} else {
					$j("#v2ray_status").append("<p style='color:red;'>V2ray未运行</p>");
				}
				$j("#v2ray_status").append("<span>当前v2ray版本:" + obj.version + "</span>");
				$j("#v2ray_status").append("<br>");
                $j("#v2ray_status").append("<span>检测时间:" + obj.date + "</span>");
			});
		}

		function isJsonString(str) {
			if (typeof str == 'string') {
				try {
					var obj = JSON.parse(str);
					if (typeof obj == 'object' && obj) {
						return obj;
					} else {
						return null;
					}

				} catch (e) {
					//					console.log('error：' + str + '!!!' + e);
					return null;
				}
			}
		}
		//加载日志内容
		function get_Log() {
			$j("#logtxt").empty();
			$j("#logtxt").load("/res/v2ray_log.htm", function (paresponseTxt, statusTxt, xhrrams) {
				if (statusTxt == "error")
					alert("Error: " + xhr.status + ": " + xhr.statusText);
			})
		}

		var enable_ss = "<% nvram_get("enable_ss"); %>";
		var enable_soft = "<% nvram_get("enable_soft"); %>";
		function menu_hook(title, tab) {
			tabtitle[tabtitle.length - 1] = new Array("", "v2ray");
			tablink[tablink.length - 1] = new Array("", "Module_v2ray.asp");
		}
	</script>
	<style>
		.contentM_qis {
			position: absolute;
			-webkit-border-radius: 5px;
			-moz-border-radius: 5px;
			border-radius: 10px;
			z-index: 10;
			background-color: #2B373B;
			margin-left: -215px;
			top: 240px;
			width: 980px;
			height: auto;
			box-shadow: 3px 3px 10px #000;
			background: rgba(0, 0, 0, 0.85);
			display: none;
		}
	</style>
</head>

<body onload="init();">
	<div id="TopBanner"></div>
	<div id="Loading" class="popup_bg"></div>
	<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>
	<form method="POST" name="form" action="/applydb.cgi?p=v2ray_" target="hidden_frame">
		<input type="hidden" name="current_page" value="Module_v2ray.asp" />
		<input type="hidden" name="next_page" value="Module_v2ray.asp" />
		<input type="hidden" name="group_id" value="" />
		<input type="hidden" name="modified" value="0" />
		<input type="hidden" name="action_mode" value="" />
		<input type="hidden" name="action_script" value="" />
		<input type="hidden" name="action_wait" value="5" />
		<input type="hidden" name="first_time" value="" />
		<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get(" preferred_lang "); %>"/>
		<input type="hidden" name="SystemCmd" onkeydown="onSubmitCtrl(this, ' Refresh ')" value="v2ray.sh" />
		<input type="hidden" name="firmver" value="<% nvram_get(" firmver "); %>"/>
		<input type="hidden" id="v2ray_enable" name="v2ray_enable" value='<% dbus_get_def("v2ray_enable", "0"); %>' />
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
											<div class="formfonttitle"><em><font face="fantasy">软件中心 - V2ray</font></em>
											</div>											<div style="float:right; width:15px; height:25px;margin-top:10px">
												<img id="return_btn" onclick="reload_Soft_Center();" align="right" style="cursor:pointer;position:absolute;margin-left:-30px;margin-top:-25px;"
												 title="返回软件中心" src="/images/backprev.png" onMouseOver="this.src='/images/backprevclick.png'" onMouseOut="this.src='/images/backprev.png'"></img>
											</div>
											<div style="margin-left:5px;margin-top:10px;margin-bottom:10px">
												<img src="/images/New_ui/export/line_export.png">
											</div>
											<div class="SimpleNote">
												<li id="push_content1" style='color:yellow;'"margin-top:-5px;"><font face="fantasy">来自网络的Project V工具.用于爬墙操作。</font></div>
											<div class="formfontdesc" id="cmdDesc"></div>
											<table style="margin:10px 0px 0px 0px;" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"
											 class="FormTable" id="kms_table">
												<thead>
													<tr>
                                                            
														<td colspan="2">主要选项</td>
													</tr>
												</thead>
												<tr>
     
													<th> V2ray开关</th>
													<td colspan="2">
														<div class="switch_field" style="display:table-cell;float: left;">
															<label for="switch">
																<input id="switch" class="switch" type="checkbox" style="display: none;">
																<div class="switch_container">
																	<div class="switch_bar"></div>
																	<div class="switch_circle transition_style">
																		<div></div>
																	</div>
																</div>
															</label>
														</div>
														<div id="kms_version_show" style="padding-top:5px;margin-left:230px;margin-top:0px;">
															<i>当前版本：
																<% dbus_get_def("softcenter_module_v2ray_version", "未知"); %>
															</i>
														</div>
														<div style="padding-top:5px;margin-left:330px;margin-top:-25px;"></div>
														<a id="v2ray_log_button" style="margin-left: 318px;" href="javascript:void(0)" target="_blank">
															<em>[
																<u> 运行日志 </u>]</em>
														</a>
													</td>
												</tr>
												<tr>
													<th>运行状态</th>
													<td id="v2ray_status">未知</td>
													
													</td>
												</tr>
												<tr>
													<th style="width:20%;"><a class="hintstyle" href="javascript:void(0);">导入V2Ray节点(json)</a></th>
													<td>
														<input style="color:#FFCC00;*color:#000;width: 200px;" id="v2ray_file" type="file" name="file" accept=".json"/>
														<span id="v2ray_file_info" style="display:block;">等待上传</span>
														<button type="button"  onclick="upload_v2ray_backup()" >上传</button>
														<button type="button"  onclick="replace_v2ray_config()">应用配置</button>
                                                                      
														<button type="submit" onclick="restart_v2ray()">重启v2ray</button>
														<button type="submit" onclick="update_v2ray()">更新v2ray</button>

 
<a href="http://ip111.cn" target="_blank">[
分流检测
]</a>
													</td>
												</tr>
											</table>

											<table style="margin:10px 0px 0px 0px;" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"
											 class="FormTable" id="kms_table">
												<thead>
													<tr>
														<td colspan="2">服务配置</td>
													</tr>
												</thead>
												<tr id="tr_v2ray_server_address">
													<th width="35%">服务器地址(Server Address)</th>
													<td>
														<input type="text" maxlength="64" id="v2ray_server_address" name="v2ray_server_address" value="" class="input_ss_table" style="width:342px;float:left;"
														 autocomplete="off" autocorrect="off" autocapitalize="off" />
													</td>
												</tr>
												<tr id="tr_v2ray_server_port">
													<th>服务器端口(port)</th>
													<td>
														<input type="text" maxlength="64" id="v2ray_server_port" name="v2ray_server_port" value="" class="input_ss_table" style="width:342px;float:left;"
														 autocomplete="off" autocorrect="off" autocapitalize="off" />
													</td>
												</tr>
												<tr id="tr_v2ray_id">
													<th width="35%">用户ID(UUID)</th>
													<td>
														<input type="text" maxlength="64" id="v2ray_id" name="v2ray_id" value="" class="input_ss_table" style="width:342px;float:left;"
														 autocomplete="off" autocorrect="off" autocapitalize="off" />
													</td>
												</tr>
												<tr id="tr_v2ray_alterid">
													<th width="35%">额外ID(alterId)</th>
													<td>
														<input type="text" maxlength="64" id="v2ray_alterid" name="v2ray_alterid" value="" class="input_ss_table" style="width:342px;float:left;"
														 autocomplete="off" autocorrect="off" autocapitalize="off" />
													</td>
												</tr>
												<tr id="tr_v2ray_security">
													<th width="35%">加密方式(security)</th>
													<td>
														<div style="float:left; width:165px; height:25px">
															<select id="v2ray_security" name="v2ray_security" style="width:164px;margin:0px 0px 0px 2px;" class="input_option">
																<option value="auto">自动识别</option>
																<option value="aes-128-cfb">aes-128-cfb</option>
																<option value="aes-128-gcm">aes-128-gcm</option>
																<option value="chacha20-poly1305">chacha20-poly1305</option>
															</select>
														</div>
													</td>
												</tr>
												<tr id="tr_v2ray_network">
													<th width="35%">传输协议(network)</th>
													<td>
														<div style="float:left; width:165px; height:25px">
															<select id="v2ray_network" name="v2ray_network" value="ws" style="width:164px;margin:0px 0px 0px 2px;" class="input_option">
																<option value="tcp">tcp</option>
																<option value="kcp">kcp</option>
																<option value="ws">ws</option>
																<option value="ws_hd">ws_headers</option>
															</select>
														</div>
													</td>
												</tr>
												<tr id="tr_v2ray_headtype" style="display: none;">
													<th width="35%">伪装类型(headtype)</th>
													<td>
														<div style="float:left; width:165px; height:25px">
															<select id="v2ray_headtype" name="v2ray_headtype" value="ws" style="width:164px;margin:0px 0px 0px 2px;" class="input_option"></select>
														</div>
													</td>
												</tr>
												<tr id="tr_v2ray_network_path" style="display: none;">
													<th width="35%">伪装路径(ws path)</th>
													<td>
														<input type="text" maxlength="64" id="v2ray_network_path" name="v2ray_network_path" value="" class="input_ss_table" style="width:342px;float:left;"
														 autocomplete="off" autocorrect="off" autocapitalize="off" />
													</td>
												</tr>
												<tr id="tr_v2ray_network_host" style="display: none;">
													<th width="35%">伪装Host(headers)</th>
													<td>
														<input type="text" maxlength="64" id="v2ray_network_host" name="v2ray_network_host" value="null" class="input_ss_table" style="width:342px;float:left;"
														 autocomplete="off" autocorrect="off" autocapitalize="off" />
													</td>
												</tr>
												<tr id="tr_v2ray_network_security">
													<th width="35%">底层传输安全(tls)</th>
													<td>
														<div style="float:left; width:165px; height:25px">
															<select id="v2ray_network_security" name="v2ray_network_security" style="width:164px;margin:0px 0px 0px 2px;" class="input_option">
																<option value="none">关闭</option>
																<option value="tls">开启tls</option>
															</select>
														</div>
													</td>
												</tr>
												<tr id="tr_v2ray_network_mux">
													<th width="35%">多路复用(mux)</th>
													<td>
														<div style="float:left; width:165px; height:25px">
															<select id="v2ray_network_mux" name="v2ray_network_mux" value="true" style="width:164px;margin:0px 0px 0px 2px;" class="input_option">
																<option value="false">关闭</option>
																<option value="true">开启mux</option>
															</select>
														</div>
													</td>
												</tr>
											</table>
											<div class="apply_gen">
												<button id="cmdBtn" class="button_gen" onclick="onSubmitCtrl(this, ' Refresh ')">保存并应用</button>
											</div>
											<div style="margin-left:5px;margin-top:10px;margin-bottom:10px">
												<img src="/images/New_ui/export/line_export.png">
											</div>
											<div id="NoteBox">
												<h2>本插件仅支持Merlin AM380 2.6.36.4内核的固件，请不要用于其它固件安装。</li>
												<h3>此插件在华硕ac68u,网件r6300v2上测试成功，其他平台请自行测试。</h3>
                                                                                               <li  
												<i>v2ray在路由器上存在启动崩溃现象，请设置客户以及服务器端alterid为较小值，例如10；
													如果使用ws+tls请将域名以及时间同步服务器地址加入域名白名单，例如xxx.example.com,
													pool.ntp.org.开启后会有守护进程检测v2ray状态,每两分钟检测一次.如果有其他需求请修改v2ray_run.sh</h3>
                                                                                                 <li 
												<i>该插件需要先启动ss,输入配置后提交，会用v2ray进程替换原ss进程,点击关闭后提交会还原ss进程，从而实现切换。</h3>
                                                                                                  <li 
												<i>本插件原理论上支持ss插件的所有功能，但只测试了v2ray的ws+tls模式，如果有问题请尝试修改生成的config.json文件,
													并打开log的debug模式进行调试。插件提供导入配置，但需要修改特定端口，详细请看：<a style="color: Yellow;" href="https://gist.github.com/wd/e0bc83b33ce63506a9bdbc3b81658c52">梅林v2ray方案</a>
												<li
												<i>守护脚本以及该插件原文件来自:<a  style="color:Yellow;" href="https://t.me/merlinv2ray">Telegram群</a></h3>
												<div style="margin-left:5px;margin-top:10px;margin-bottom:10px">
													<img src="/images/New_ui/export/line_export.png">
												</div>
												  <li  id="push_content1" style="margin-top:-5px;">参考链接:
												<a href="https://www.v2ray.com">
													<i style="margin-right:6px">V2ray官网</i>
												</a>
												<a href="https://github.com/v2ray/v2ray-core">
													<i style="margin-right:6px">V2ray的Github</i>
												</a>
												<a href="https://www.v2ray.com/chapter_02/protocols/vmess.html">
													<i style="margin-right:6px">配置名词解释</i>
												</a>
												<a href="https://233blog.com/post/17/">
													<i style="margin-right:6px">服务器一键脚本</i>
												</a>
												<a href="https://htfy96.github.io/v2ray-config-gen/">
													<i style="margin-right:6px">配置文件可视化生成</i>
												</a>
                                     <a href="https://github.com/v2ray/v2ray-core/releases">
													<i style="margin-right:6px">v2ray客户端下载
                                     <li 
                                                      <h2> 我们的征途是星辰大海 ^_^ ^_^ ^_^
                                                                                         </a>
											</div>
											<div style="margin-left:5px;margin-top:10px;margin-bottom:10px">
												<img src="/images/New_ui/export/line_export.png">
											</div>
											<div class="KoolshareBottom">
												<br/>论坛技术支持：
												<a href="http://www.koolshare.cn" target="_blank">
													<i>
														<u>www.koolshare.cn</u>
													</i>
												</a>
												<br/>后台技术支持：
												<i>Xiaobao</i>
												<br/>Shell, Web by：
												<i>fw867,clarence</i>
												<br/>
											</div>
										</td>
									</tr>
								</table>
								<!-- this is the popup area for user rules -->
								<div id="vpnc_settings" class="contentM_qis" style="box-shadow: 3px 3px 10px #000;margin-top: -65px;">
									<div style="text-align: center;font-size: 18px;color: #99FF00;padding: 10px;font-weight: bold;">V2ray运行日志</div>
									<div id="user_tr" style="margin: 10px 10px 10px 10px;width:98%;text-align:center;">
										<textarea cols="63" rows="36" wrap="off" id="logtxt" style="width:97%;padding-left:10px;padding-right:10px;border:1px solid #222;font-family:'Courier New', Courier, mono; font-size:11px;background:#475A5F;color:#FFFFFF;outline: none;"
										 autocomplete="off" autocorrect="off" autocapitalize="off" spellcheck="false">
                                        </textarea>
									</div>
									<div style="margin-top:5px;padding-bottom:10px;width:100%;text-align:center;">
										<input id="close_log_win" class="button_gen" type="button" value="返回主界面">
									</div>
								</div>
								<!-- end of the popouparea -->
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
