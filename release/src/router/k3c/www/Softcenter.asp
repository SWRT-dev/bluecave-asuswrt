<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=Edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<title>梅林软件中心</title>
<link rel="stylesheet" type="text/css" href="index_style.css">
<link rel="stylesheet" type="text/css" href="form_style.css">
<script type="text/javascript" src="/state.js"></script>
<script type="text/javascript" src="/help.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script language="JavaScript" type="text/javascript" src="/validator.js"></script>
<script type="text/javascript" src="/js/jquery.js"></script>
<script type="text/javascript" src="/general.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<script type="text/javascript" src="/form.js"></script>
<style>
.cloud_main_radius_left{
    -webkit-border-radius: 10px 0 0 10px;
    -moz-border-radius: 10px 0 0 10px;
    border-radius: 10px 0 0 10px;
}
.cloud_main_radius_right{
    -webkit-border-radius: 0 10px 10px 0;
    -moz-border-radius: 0 10px 10px 0;
    border-radius: 0 10px 10px 0;
}
.cloud_main_radius{
    -webkit-border-radius: 10px;
    -moz-border-radius: 10px;
    border-radius: 10px;
}
</style>
<style>
    /* 软件中心icon新样式 by acelan */
    dl,dt,dd{
        padding:0;
        margin:0;
    }
    input[type=button]:focus {
        outline: none;
    }
   .icon{
        float:left;
        position:relative;
        margin: 10px 0px 30px 0px;
    }
    .icon-title{
        line-height: 3em;
        text-align:center;
    }
    .icon-pic{
        margin: 10px 30px 0px 30px;
    }
    .icon-pic img{
        border:0;
        width: 60px;
        height: 60px;
        margin:2px;
    }
    .icon-desc{
        position: absolute;
        left: 0;
        top: 0;
        height: 105%;
        visibility: hidden;
        font-size:0;
        width: 119px;
        border-radius: 8px;
        font-size: 16px;
        opacity: 0;
        background-color:#000;
        margin:5px;
        text-overflow:ellipsis;
        transition: opacity .5s ease-in;
    }
    .icon-desc .text{
        font-size: 12px;
        line-height: 1.4em;
        display: block;
        height: 100%;
        padding: 10px;
        box-sizing: border-box;
    }
    .icon:hover .icon-desc{
        opacity: .8;
        visibility: visible;
    }
    .icon-desc .opt{
        position: absolute;
        bottom: 0;
        height: 18px;
        width: 100%;
    }
    .install-status-0 .icon-desc .opt{
        height: 100%;
    }
    .icon-desc .install-btn,
    .icon-desc .uninstall-btn,
    .icon-desc .update-btn{
    	background: #fff;
    	color:#333;
    	cursor:pointer;
    	text-align: center;
    	font-size: 13px;
    	padding-bottom: 5px;
    	margin-left: 10px;
    	margin-right: 10px;
        display: block;
        width: 100%;
        height: 18px;
        border-radius: 0px 0px 5px 5px;
        border: 0px;
        position: absolute;
        bottom: 0;
        left: -10px;
    }

    .icon-desc .uninstall-btn{
        display: none;
    }
    .icon-desc .update-btn{
        display: none;
        border-radius: 0px 0px 0px 5px;
        width:60%;
        border-right: 1px solid #000;
    }
    .show-install-btn,
    .show-uninstall-btn{
        border: none;
        background: #444;
        color: #fff;
        padding: 10px 20px;
        border-radius: 5px 5px 0px 0px;
    }
    .active{
        background: #444f53;
    }
    .install-status-1 .uninstall-btn{
        display: block;
    }
    .install-status-1 .install-btn{
        display: none;
    }
    .update-btn{
        display: none;
    }
    .install-status-1 .update-btn{
        display: none;
    }
    .install-status-4 .uninstall-btn{
        display: block;
    }
    .install-status-4 .install-btn{
        display: none;
    }
    .install-status-4 .update-btn{
        display: none;
    }
    .install-status-2 .uninstall-btn{
        display: block;
        width: 40%;
        border-radius: 0px 0px 5px 0px;
        right: -10px;
        left: auto;
        border-left: 1px solid #000;
    }
    .install-status-2 .install-btn{
        display: none;
    }
    .install-status-2 .update-btn{
        display: block;
    }
    .install-status-1{
        display: none;
    }
    .install-status-2{
        display: none;
    }
    .install-status-0{
        display: block;
    }
    .install-status-4{
        display: none;
    }
    .install-view .install-status-1{
        display: block;
    }
    .install-view .install-status-2{
        display: block;
    }
    .install-view .install-status-0{
        display: none;
    }
    .install-view .install-status-4{
        display: block;
    } 
    .cloud_main_radius h2 { border-bottom:1px #AAA dashed;}
	.cloud_main_radius h3,
	.cloud_main_radius h4 { font-size:12px;color:#FC0;font-weight:normal;font-style: normal;}
	.cloud_main_radius h5 { color:#FFF;font-weight:normal;font-style: normal;}
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
</script>
</head>
<body onload="initial();" onunload="unload_body();" onselectstart="return false;">
    <div id="TopBanner"></div>
    <div id="Loading" class="popup_bg"></div>
<iframe name="hidden_frame" id="hidden_frame" width="0" height="0" frameborder="0"></iframe>
<form method="post" name="form" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="productid" value="<% nvram_get("productid"); %>">
    <input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>"/>
    <input type="hidden" name="current_page" value="Main_Soft_center.asp">
    <input type="hidden" name="next_page" value="Main_Soft_center.asp">
    <input type="hidden" name="group_id" value="">
    <input type="hidden" name="modified" value="0">
    <input type="hidden" name="action_mode" value="">
    <input type="hidden" name="action_script" value="">
    <input type="hidden" name="action_wait" value="8">
    <input type="hidden" name="first_time" value="">
    <input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>">
    <input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>">
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
                            <div>
                                <table width="760px" border="0" cellpadding="5" cellspacing="0" bordercolor="#6b8fa3" class="FormTitle" id="FormTitle">
                                    <tr>
                                        <td bgcolor="#4D595D" colspan="3" valign="top">
                                            <div>&nbsp;</div>
                                            <div class="formfonttitle">软件中心</div>
                                            <div style="margin-left:5px;margin-top:5px;margin-bottom:5px"><img src="/images/New_ui/export/line_export.png"></div>
                                                <table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3" class="FormTable" >
                                                </table>
                                                <table width="100%" height="150px" style="border-collapse:collapse;">
                                                    <tr bgcolor="#444f53">
                                                        <td colspan="5" bgcolor="#444f53" class="cloud_main_radius">
                                                            <div style="padding:10px;width:95%;font-style:italic;font-size:14px;">
                                                                <br/><br/>
                                                                <table width="100%" >
                                                                    <tr>
                                                                        <td>
                                                                            <ul style="margin-top:-50px;padding-left:15px;" >
                                                                                <li style="margin-top:-5px;">
                                                                                    <h2 id="push_titile"><em>欢迎</em></h2>
                                                                                </li>
                                                                                <li style="margin-top:-5px;">
                                                                                    <h4 id="push_content1" >欢迎来到插件中心，目前正在紧张开发中，各种插件酝酿中！</h4>
                                                                                </li>
                                                                                <li  style="margin-top:-5px;">
                                                                                    <h4 id="push_content2">所有插件需要插入U盘或移动硬盘才能启用！</h4>
                                                                                </li>
                                                                                <li  style="margin-top:-5px;">
                                                                                    <h4 id="push_content3">为保证正常运行，U盘或移动硬盘第一分区至少需分配1G空间留给系统使用！</h4>
                                                                                </li>
                                                                                </li>
                                                                            </ul>
                                                                        </td>
                                                                    </tr>
                                                                </table>
                                                            </div>
                                                        </td>
                                                    </tr>
                                                    <tr height="10px">
                                                        <td colspan="3"></td>
                                                    </tr>

                                                    <tr bgcolor="#444f53" id="install_status" style="display: none;" width="235px">
                                                        <td>
                                                            <div style="padding:10px;width:95%;font-size:14px;" id="appInstallInfo">
                                                            </div>
                                                        </td>
                                                        <td class="cloud_main_radius_right">
                                                        </td>
                                                     </tr>
                                                    <tr height="10px">
                                                        <td colspan="3"></td>
                                                    </tr>
                                                    <tr width="235px">
                                                        <td colspan="4" cellpadding="0" cellspacing="0" style="padding:0">
                                                            <input class="show-install-btn" type="button" value="已安装"/>

                                                    </tr>

                                                    <tr bgcolor="#444f53" width="235px">
                                                        <td colspan="4" id="IconContainer">
                                                            <div style="text-align:center; line-height: 4em;">
																	<!--模板：下面添加插件-->
																	<dl class="icon install-status-ssr" data-name="ssr">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/ssr.png">
																	</dd>
																	<dt class="icon-title">科学上网</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_SSR.asp">
																			科学上网
																		</a>
																	</dd>
																	</dl>
																	<!--模板：添加结束-->
																	<dl class="icon install-status-kp" data-name="kp">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/koolproxy.png">
																	</dd>
																	<dt class="icon-title">广告屏蔽</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_Adbyby.asp">
																			广告屏蔽大师
																		</a>
																	</dd>
																	</dl>
																	<dl class="icon install-status-kms" data-name="kms">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/microsoft.png">
																	</dd>
																	<dt class="icon-title">KMS服务</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_Kms.asp">
																			KMS服务
																		</a>
																	</dd>
																	</dl>
																	<dl class="icon install-status-ngrok" data-name="ngrok">
																	<dd class="icon-pic">
																		<img src="">
																	</dd>
																	<dt class="icon-title">内网穿透Ngrok</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_ngrok.asp">
																			内网穿透Ngrok
																		</a>
																	</dd>
																	</dl>
																	<dl class="icon install-status-frp" data-name="frp">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/frpc.png">
																	</dd>
																	<dt class="icon-title">内网穿透Frpc</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_frpc.asp">
																			内网穿透Frpc
																		</a>
																	</dd>
																	</dl>
																	<dl class="icon install-status-webshell" data-name="webshell">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/webshell.png">
																	</dd>
																	<dt class="icon-title">Webshell</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_webshell.asp">
																			超强的SSH网页客户端~
																		</a>
																	</dd>
																	</dl>
																	<dl class="icon install-status-xunlei" data-name="xunlei">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/xunlei.png">
																	</dd>
																	<dt class="icon-title">迅雷远程</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_xunlei.asp">
																			迅雷远程
																		</a>
																	</dd>
																	</dl>
																	<!--<dl class="icon install-status-swap" data-name="swap">
																	<dd class="icon-pic">
																		<img src="/images/New_ui/swap.png">
																	</dd>
																	<dt class="icon-title">虚拟内存</dt>
																	<dd class="icon-desc">
																		<a class="text" href="/Tools_swap.asp">
																			老板，来一斤虚拟内存~
																		</a>
																	</dd>
																	</dl>-->
                                                            </div>
                                                        </td>
                                                    </tr>
                                                    <tr height="10px">
                                                        <td colspan="3"></td>
                                                    </tr>

                                                </table>
                                            <div class="KoolshareBottom">论坛技术支持： <a href="http://www.koolshare.cn" target="_blank"> <i><u>www.koolshare.cn</u></i> </a>
                                                <br/>Github项目： <a href="https://github.com/koolshare/koolshare.github.io" target="_blank"> <i><u>github.com/koolshare</u></i> </a>
                                            	<br/>K3C修改版 by： <a href="https://k3c.paldier.tk/merlin" target="_blank"><i><u>paldier</u></i></a>
											</div>
                                        </td>
                                    </tr>
                            </table>
                        </div>
                    </td>
                </tr>
            </table>
        </td>
        <td width="10" align="center" valign="top"></td>
    </tr>
</table>
<div id="footer"></div>
</body>
</html>
