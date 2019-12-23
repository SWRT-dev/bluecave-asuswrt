var dict = {};
//var systemLang = navigator.language.toLowerCase().slice(0,2);
var web_lang= "<% nvram_get("preferred_lang"); %>";
function sc_load_lang(path) {
  registerWords();
    switch (web_lang) {
    case "EN":
      setLanguage("EN",path);
      break
    case "TW":
      setLanguage("TW",path);
      break
    case "CN":
      setLanguage("CN",path);
      break
    default:
      setLanguage("EN",path);
  }
}

function setLanguage(lang,path) {
  translate(lang,path);
}

function translate(lang,path) {
  if(sessionStorage.getItem(lang + "Data") != null){
    dict = JSON.parse(sessionStorage.getItem(lang + "Data"));
  }else{
    loadDict(path);
  }

  $("[sclang]").each(function () {
    switch (this.tagName.toLowerCase()) {
      case "input":
        $(this).val(__tr($(this).attr("sclang")));
        break;
      default:
        $(this).text(__tr($(this).attr("sclang")));
    }
  });
  $("[lang-input]").each(function () {
    $(this).attr("placeholder", __tr($(this).attr("lang-input")));
  });
}

function __tr(src) {
  return (dict[src] || src);
}

function loadDict(path) {
  var lang = (web_lang || "EN");
  $.ajax({
    async: false,
    type: "GET",
    url: "/res/"+ path + lang + ".json",
    success: function (msg) {
      dict = msg;
      sessionStorage.setItem(lang + 'Data', JSON.stringify(dict));
    }
  });

}

// 遍历所有lang属性的标签赋值
function registerWords() {
  $("[sclang]").each(function () {
    if ($(this).attr("sclang") === "") {
      switch (this.tagName.toLowerCase()) {
        case "input":
          $(this).attr("sclang", $(this).val());
          break;
        default:
          $(this).attr("sclang", $(this).text());
      }
    }
  });
   $("[lang-input]").each(function () {
    if ($(this).attr("lang-input") === "") {
      $(this).attr("lang-input", $(this).attr("placeholder"));
    }
  });
}

