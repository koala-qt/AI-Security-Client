webpackJsonp([7],{"9bBU":function(e,t,s){s("mClu");var a=s("FeBl").Object;e.exports=function(e,t,s){return a.defineProperty(e,t,s)}},C4MV:function(e,t,s){e.exports={default:s("9bBU"),__esModule:!0}},bOdI:function(e,t,s){"use strict";t.__esModule=!0;var a,i=s("C4MV"),l=(a=i)&&a.__esModule?a:{default:a};t.default=function(e,t,s){return t in e?(0,l.default)(e,t,{value:s,enumerable:!0,configurable:!0,writable:!0}):e[t]=s,e}},eglA:function(e,t,s){"use strict";Object.defineProperty(t,"__esModule",{value:!0});var a=s("//Fk"),i=s.n(a),l=s("bOdI"),n=s.n(l),o=s("2hMI"),c={data:function(){var e;return e={recordData:[],currentPage:1,total:1,dateTime:"",passageResult:"0,1",personType:"",divergenceType:"0,1",name:"",code:"",typeList:[],typeAll:"",pageNumber:1},n()(e,"total",1),n()(e,"pageSize",5),n()(e,"ids",[]),n()(e,"getRowKeys",function(e){return e.id}),n()(e,"detailInfo",{}),n()(e,"detailsDialog",!1),e},created:function(){var e=this;Date.prototype.Format=function(e){var t={"M+":this.getMonth()+1,"d+":this.getDate(),"h+":this.getHours(),"m+":this.getMinutes(),"s+":this.getSeconds(),"q+":Math.floor((this.getMonth()+3)/3),S:this.getMilliseconds()};for(var s in/(y+)/.test(e)&&(e=e.replace(RegExp.$1,(this.getFullYear()+"").substr(4-RegExp.$1.length))),t)new RegExp("("+s+")").test(e)&&(e=e.replace(RegExp.$1,1==RegExp.$1.length?t[s]:("00"+t[s]).substr((""+t[s]).length)));return e},this.dateTime=[(new Date).Format("yyyy-MM-dd 00:00:00"),(new Date).Format("yyyy-MM-dd 23:59:59")],this.netAPI=Object(o.a)().get("accessSystem"),new i.a(function(t,s){e.$http.get(e.netAPI.accessDevice.findPeopleType).then(function(s){if(200==s.body.status){for(var a in t(s.body.data),e.typeList=s.body.data,e.typeList)if(e.typeList.hasOwnProperty(a)){var i=e.typeList[a].categoryNo;e.typeAll+=i+","}console.log(e.typeAll),e.personType=e.typeAll}else e.$message.error(s.body.message)})}).then(function(t){e.getRecordList()})},methods:{showDetail:function(e){var t=this;this.$http.get(this.netAPI.alert.peopleDetail,{params:{id:e.id}}).then(function(e){200==e.body.status?(t.$set(t.detailInfo,"urls",e.body.data.urls[0]),console.log(t.detailInfo)):t.$message.error(e.body.message)}),this.detailInfo=e,this.detailsDialog=!0},detailsDialogClose:function(){this.detailInfo={},this.detailsDialog=!1},formatValidite:function(e,t){return 1==e.validateResult?this.$t("accessSystem.tongguo"):0==e.sex?this.$t("accessSystem.weitongguo"):this.$t("accessSystem.weizhiz")},formatDivergence:function(e,t){return 1==e.divergenceType?this.$t("accessSystem.jin"):0==e.divergenceType?this.$t("accessSystem.chu"):this.$t("accessSystem.weizhiz")},exportExcel:function(){var e=this;for(var t in this.multipleSelection)if(this.multipleSelection.hasOwnProperty(t)){var s=this.multipleSelection[t];this.ids+=s.id+","}console.log(this.ids);var a={ids:this.ids};console.log(a),this.$http({url:this.netAPI.accessRecords.accessExport,method:"get",params:a,responseType:"blob"}).then(function(t){var s=e.$t("accessSystem.daochuwenjian");console.log(t);var a=new Blob([t.body],{type:"application/octet-stream"});if(window.navigator.msSaveOrOpenBlob)navigator.msSaveBlob(a,s);else{var i=document.createElement("a");i.href=window.URL.createObjectURL(a),i.download=s,document.body.appendChild(i);var l=document.createEvent("MouseEvents");l.initEvent("click",!1,!1),i.dispatchEvent(l),document.body.removeChild(i)}})},handleSelectionChange:function(e){e.length?this.multipleSelection=e:this.multipleSelection=[]},removeRecord:function(e){var t=this;if(console.log(e),console.log(this.multipleSelection),e.id)console.log(1),this.ids=[{id:e.id}];else for(var s in this.multipleSelection)if(this.multipleSelection.hasOwnProperty(s)){var a=this.multipleSelection[s];this.ids.push({id:a.id})}0==this.ids.length?this.$message.error(this.$t("accessSystem.qingxuanzeshanchuxiang")):this.$http.post(this.netAPI.accessRecords.accessRemove,this.ids).then(function(e){200==e.body.status&&(t.$message.success(t.$t("accessSystem.shanchuchenggong")),1==t.recordData.length&&(t.pageNumber=t.pageNumber-1>0?t.pageNumber-1:1),t.getRecordList())})},handleSizeChange:function(e){this.pageSize=e,this.getRecordList()},handleCurrentChange:function(e){this.pageNumber=e,this.getRecordList()},getRecordList:function(){var e=this;console.log(this.dateTime);var t={startDate:this.dateTime[0],endDate:this.dateTime[1],passageResult:this.passageResult,personType:this.personType,divergenceType:this.divergenceType,name:this.name,code:this.code,pageNo:this.pageNumber,pageSize:this.pageSize};this.$http.get(this.netAPI.accessRecords.accessList,{params:t}).then(function(t){200==t.body.status?(e.recordData=t.body.data,e.total=t.body.total):e.$message.error(t.body.message)})},getTypeList:function(){var e=this;this.$http.get(this.netAPI.accessDevice.findPeopleType).then(function(t){if(200==t.body.status){for(var s in e.typeList=t.body.data,e.typeList)if(e.typeList.hasOwnProperty(s)){var a=e.typeList[s].categoryNo;e.typeAll+=a+","}console.log(e.typeAll),e.personType=e.typeAll}else e.$message.error(t.body.message)})}}},r={render:function(){var e=this,t=e.$createElement,s=e._self._c||t;return s("div",{attrs:{id:"accessRecords"}},[s("el-dialog",{attrs:{title:e.$t("accessSystem.xiangqing"),visible:e.detailsDialog,width:"35%","before-close":e.detailsDialogClose},on:{"update:visible":function(t){e.detailsDialog=t}}},[s("el-row",["陌生人闯入告警"!=e.detailInfo.type?s("el-col",{attrs:{span:8}},[s("div",{staticClass:"grid-contentFir"},[s("img",{staticClass:"snapduibi",attrs:{src:e.detailInfo.urls,alt:"暂无"}})])]):e._e(),e._v(" "),"陌生人闯入告警"!=e.detailInfo.type?s("el-col",{staticStyle:{"margin-top":"100px"},attrs:{span:8}},[s("div",{staticClass:"grid-contentFir"},[s("span",{staticClass:"score"},[e._v(e._s(100*e.detailInfo.score+"%"))])])]):e._e(),e._v(" "),s("el-col",{attrs:{span:8}},[s("div",{staticClass:"grid-contentFir"},[s("img",{staticClass:"snapduibi",attrs:{src:e.detailInfo.snapPhoto}})])])],1),e._v(" "),s("el-row",[s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.renyuanxingming"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.name))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.renyuanbianhao"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.id))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.shenfenzhenghao"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.code))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.renyuanleixing"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.personType))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.tongxingshijian"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.recordDate))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.tongxingshebei"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.deviceName))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.jinchuleixing"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.divergenceType))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.tongxingfangshi"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.passageWay))])]),e._v(" "),s("div",{staticClass:"grid-content "},[e._v(e._s(e.$t("accessSystem.yanzhengjieguo"))+":"),s("label",{staticClass:"value"},[e._v(e._s(e.detailInfo.validateResult))])])]),e._v(" "),s("span",{staticClass:"dialog-footer",attrs:{slot:"footer"},slot:"footer"},[s("el-button",{on:{click:function(t){e.detailsDialog=!1}}},[e._v(e._s(e.$t("accessSystem.guanbi")))])],1)],1),e._v(" "),s("div",{staticClass:"contantHeader"},[s("span",{staticClass:"title"},[e._v(e._s(e.$t("accessSystem.tongxingjilu")))]),e._v(" "),s("span",{staticStyle:{"font-size":"14px","margin-left":"1.7rem"}},[e._v(e._s(e.$t("accessSystem.jilushijianduan"))+":")]),e._v(" "),s("el-date-picker",{attrs:{type:"datetimerange",align:"right","start-placeholder":e.$t("accessSystem.kaishiriqi"),"end-placeholder":e.$t("accessSystem.jieshuriqi"),"default-time":["12:00:00","08:00:00"],"value-format":"yyyy-MM-dd HH:mm:ss","unlink-panels":""},model:{value:e.dateTime,callback:function(t){e.dateTime=t},expression:"dateTime"}}),e._v(" "),s("span",{staticStyle:{"font-size":"14px","margin-left":"0.32rem"}},[e._v(e._s(e.$t("accessSystem.tongxingjieguo"))+":")]),e._v(" "),s("el-select",{staticClass:"select",attrs:{placeholder:e.$t("accessSystem.qingxuanze")},model:{value:e.passageResult,callback:function(t){e.passageResult=t},expression:"passageResult"}},[s("el-option",{key:"全部",attrs:{label:e.$t("accessSystem.quanbu"),value:"0,1"}}),e._v(" "),s("el-option",{key:"通过",attrs:{label:e.$t("accessSystem.tongguo"),value:"1"}}),e._v(" "),s("el-option",{key:"未通过",attrs:{label:e.$t("accessSystem.weitongguo"),value:"0"}})],1),e._v(" "),s("span",{staticStyle:{"font-size":"14px","margin-left":"0.32rem"}},[e._v(e._s(e.$t("accessSystem.renyuanleixing"))+":")]),e._v(" "),s("el-select",{staticClass:"select",attrs:{placeholder:e.$t("accessSystem.qingxuanze")},model:{value:e.personType,callback:function(t){e.personType=t},expression:"personType"}},[s("el-option",{key:e.typeAll,attrs:{label:e.$t("accessSystem.quanbu"),value:e.typeAll}}),e._v(" "),e._l(e.typeList,function(e){return s("el-option",{key:e.categoryNo,attrs:{label:e.categoryName,value:e.categoryNo}})})],2),e._v(" "),s("span",{staticStyle:{"font-size":"14px","margin-left":"0.32rem"}},[e._v(e._s(e.$t("accessSystem.jinchuleixing"))+"：")]),e._v(" "),s("el-select",{staticClass:"select",staticStyle:{width:"100px"},attrs:{placeholder:e.$t("accessSystem.qingxuanze")},model:{value:e.divergenceType,callback:function(t){e.divergenceType=t},expression:"divergenceType"}},[s("el-option",{key:"全部",attrs:{label:e.$t("accessSystem.quanbu"),value:"0,1"}}),e._v(" "),s("el-option",{key:"进",attrs:{label:e.$t("accessSystem.jin"),value:"1"}}),e._v(" "),s("el-option",{key:"出",attrs:{label:e.$t("accessSystem.chu"),value:"0"}})],1),e._v(" "),s("el-input",{staticClass:"input",staticStyle:{"margin-left":"0.5rem"},attrs:{placeholder:e.$t("accessSystem.xingming"),clearable:"",maxlength:"20"},model:{value:e.name,callback:function(t){e.name=t},expression:"name"}}),e._v(" "),s("el-input",{staticClass:"input",attrs:{placeholder:e.$t("accessSystem.bianhao"),clearable:"",maxlength:"20"},model:{value:e.code,callback:function(t){e.code=t},expression:"code"}}),e._v(" "),s("div",{staticStyle:{float:"right","margin-right":"0.7rem","margin-top":"10px"}},[s("el-button",{attrs:{type:"primary"},on:{click:e.getRecordList}},[e._v(e._s(e.$t("accessSystem.chaxun")))]),e._v(" "),s("el-button",{attrs:{type:"primary"},on:{click:e.removeRecord}},[e._v(e._s(e.$t("accessSystem.shanchu")))]),e._v(" "),s("el-button",{attrs:{type:"primary"},on:{click:e.exportExcel}},[e._v(e._s(e.$t("accessSystem.daochu")))])],1)],1),e._v(" "),s("div",{staticClass:"contantTable"},[s("el-table",{staticStyle:{width:"100%"},attrs:{data:e.recordData,"row-key":e.getRowKeys},on:{"selection-change":e.handleSelectionChange}},[s("el-table-column",{attrs:{type:"selection",width:"55","reserve-selection":!0}}),e._v(" "),s("el-table-column",{attrs:{type:"index",label:e.$t("accessSystem.xuhao"),width:"50"}}),e._v(" "),s("el-table-column",{attrs:{prop:"name",label:e.$t("accessSystem.xingming")}}),e._v(" "),s("el-table-column",{attrs:{prop:"id",label:e.$t("accessSystem.renyuanbianhao")}}),e._v(" "),s("el-table-column",{attrs:{prop:"personType",label:e.$t("accessSystem.renyuanleixing")}}),e._v(" "),s("el-table-column",{attrs:{prop:"recordDate",label:e.$t("accessSystem.shijian")}}),e._v(" "),s("el-table-column",{attrs:{prop:"deviceName",label:e.$t("accessSystem.shebei")}}),e._v(" "),s("el-table-column",{attrs:{prop:"passageWay",label:e.$t("accessSystem.tongxingfangshi")}}),e._v(" "),s("el-table-column",{attrs:{prop:"validateResult",label:e.$t("accessSystem.yanzhengjieguo"),formatter:e.formatValidite}}),e._v(" "),s("el-table-column",{attrs:{prop:"passageResult",label:e.$t("accessSystem.tongxingjieguo"),formatter:e.formatValidite}}),e._v(" "),s("el-table-column",{attrs:{prop:"score",label:e.$t("accessSystem.duibidafen")}}),e._v(" "),s("el-table-column",{attrs:{prop:"divergenceType",label:e.$t("accessSystem.jinchuleixing"),formatter:e.formatDivergence}}),e._v(" "),s("el-table-column",{attrs:{label:e.$t("accessSystem.zhuapaizhaopian")},scopedSlots:e._u([{key:"default",fn:function(e){return[s("img",{staticClass:"snapPhoto",attrs:{src:e.row.snapPhoto,alt:""}})]}}])}),e._v(" "),s("el-table-column",{attrs:{label:e.$t("accessSystem.caozuo")},scopedSlots:e._u([{key:"default",fn:function(t){return[s("el-button",{attrs:{size:"mini",type:"text"},on:{click:function(s){e.showDetail(t.row)}}},[e._v(e._s(e.$t("accessSystem.xiangqing")))]),e._v(" "),s("el-button",{attrs:{size:"mini",type:"text"},on:{click:function(s){e.removeRecord(t.row)}}},[e._v(e._s(e.$t("accessSystem.shanchu")))])]}}])})],1),e._v(" "),s("el-pagination",{staticClass:"pagination",attrs:{"current-page":e.pageNumber,"page-sizes":[5,10,20,40],"page-size":e.pageSize,layout:"total, sizes, prev, pager, next, jumper",total:e.total},on:{"size-change":e.handleSizeChange,"current-change":e.handleCurrentChange}})],1)],1)},staticRenderFns:[]};var d=s("VU/8")(c,r,!1,function(e){s("xwIM")},"data-v-29bce440",null);t.default=d.exports},mClu:function(e,t,s){var a=s("kM2E");a(a.S+a.F*!s("+E39"),"Object",{defineProperty:s("evD5").f})},xwIM:function(e,t){}});
//# sourceMappingURL=7.b78e457164a4c8e49570.js.map