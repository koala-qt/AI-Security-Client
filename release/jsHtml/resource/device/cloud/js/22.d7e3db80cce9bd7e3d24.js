webpackJsonp([22],{"6lr+":function(e,s,t){"use strict";Object.defineProperty(s,"__esModule",{value:!0});var i=t("mvHQ"),a=t.n(i),o=t("//Fk"),n=t.n(o),c=t("2hMI"),r={data:function(){return{accessDeviceList:[],accessDeviceIds:[],deviceConnectVisible:!1,netAPI:{},form:{name:"",description:"",personType:[],allow:1,passageMode:"",passageModes:[]},getRowKeys:function(e){return e.id},total:1,pageSize:5,pageNumber:1,rulesData:[],dialogAttrVisible:!1,allWeeks:[{name:this.$t("accessSystem.zhouyi"),timeList:[{timeQj:[0,144]}]},{name:this.$t("accessSystem.zhouer"),timeList:[{timeQj:[0,144]}]},{name:this.$t("accessSystem.zhousan"),timeList:[{timeQj:[0,144]}]},{name:this.$t("accessSystem.zhousi"),timeList:[{timeQj:[0,144]}]},{name:this.$t("accessSystem.zhouwu"),timeList:[{timeQj:[0,144]}]},{name:this.$t("accessSystem.zhouliu"),timeList:[{timeQj:[0,144]}]},{name:this.$t("accessSystem.zhouri"),timeList:[{timeQj:[0,144]}]}],PeopleTypeList:[],peopleValidList:[],WayList:{},passWayList:{},dialogType:"",isEdit:!1,multipleSelection:[],rulesInfo:{}}},computed:{rules:function(){return{name:[{required:!0,message:this.$t("accessSystem.qingshurushebeimingcheng"),trigger:"change"}],personType:[{required:!0,message:this.$t("accessSystem.qingxuanzerenyuanleixing"),trigger:"change"}],passageMode:[{required:!0,message:this.$t("accessSystem.qingxuanzetongxingfangshi"),trigger:"change"}]}}},created:function(){var e=this;this.netAPI=Object(c.a)().get("accessSystem"),new n.a(function(s,t){e.$http.get(e.netAPI.accessDevice.findPeopleType).then(function(t){200==t.body.status?(s(t.body.data),e.PeopleTypeList=t.body.data):e.$message.error(t.body.message)})}).then(function(s){e.getRulesList()}),this.getPassageWay(),this.getAccessDeviceList()},methods:{closeDialog:function(){this.dialogAttrVisible=!1},timeDataTransfer:function(e){var s=this,t=e;this.allWeeks.forEach(function(e,t){s.allWeeks[t].timeList&&(s.allWeeks[t].timeList=[])});var i=["mon","tue","wed","thu","fri","sat","sun"];i.forEach(function(e,a){t[i[a]].split(",").forEach(function(e,t){console.log(e);var i=6*parseInt(e.split("-")[0].split(":")[0])+parseInt(e.split("-")[0].split(":")[1])/10,o=6*parseInt(e.split("-")[1].split(":")[0])+parseInt(e.split("-")[1].split(":")[1])/10;s.allWeeks[a].timeList.push({timeQj:[i,o]})})})},disconnectDevice:function(e,s){var t=this;console.log(e),this.$http.post(this.netAPI.accessRules.connectDevice,[{mappingId:e.id}]).then(function(i){200==i.body.status?(t.$message.success(t.$t("accessSystem.shanchuchenggong")),s.splice(s.indexOf(e),1),t.getRulesList()):t.$message.error(i.body.message)})},handleSelectionChange:function(e){console.log(e),e.length?this.multipleSelection=e:this.multipleSelection=[]},connectShow:function(e){for(var s in this.accessDeviceIds=[],this.deviceConnectVisible=!0,this.rulesInfo=e,e.ruleDeviceVos)if(e.ruleDeviceVos.hasOwnProperty(s)){var t=e.ruleDeviceVos[s];this.accessDeviceIds.push(t.deviceId)}for(var i in this.accessDeviceList)if(this.accessDeviceList.hasOwnProperty(i)){var a=this.accessDeviceList[i];for(var o in a.disabled=!1,e.ruleDeviceVos)if(e.ruleDeviceVos.hasOwnProperty(o)){var n=e.ruleDeviceVos[o];a.key==n.deviceId&&(a.disabled=!0)}}},deviceClose:function(){var e=this;console.log(this.accessDeviceIds),console.log(this.multipleSelection);var s=[];for(var t in this.accessDeviceIds)if(this.accessDeviceIds.hasOwnProperty(t)){var i=this.accessDeviceIds[t];for(var a in s.push({id:this.rulesInfo.id,deviceId:i}),this.rulesInfo.ruleDeviceVos){if(this.rulesInfo.ruleDeviceVos.hasOwnProperty(a))this.rulesInfo.ruleDeviceVos[a].deviceId==i&&s.pop()}}this.$http.post(this.netAPI.accessRules.connectDevice,s).then(function(s){200==s.body.status?(e.$message.success(e.$t("accessSystem.xiafachenggong")),e.deviceConnectVisible=!1,e.getRulesList()):e.$message.error(s.body.message)})},resetDevice:function(){this.accessDeviceIds=[],this.deviceConnectVisible=!1},getAccessDeviceList:function(){var e=this;this.$http.get(this.netAPI.accessDevice.findDeviceAll,{params:{pageNo:1,pageSize:1e3}}).then(function(s){if(200==s.body.status){for(var t in s.body.data)if(s.body.data.hasOwnProperty(t)){var i=s.body.data[t];e.accessDeviceList.push({key:i.deviceId,label:i.deviceName})}}else e.$message.error(s.body.message)})},deleteRule:function(e){var s=this;this.$http.post(this.netAPI.accessRules.rulesRemove,[{id:e.id}]).then(function(e){200==e.body.status?s.$message.success(s.$t("accessSystem.shanchuchenggong")):s.$message.error(e.body.message)}),console.log(e);var t=[];for(var i in e.ruleDeviceVos)if(e.ruleDeviceVos.hasOwnProperty(i)){var a=e.ruleDeviceVos[i];t.push({mappingId:a.id})}console.log(t),this.$http.post(this.netAPI.accessRules.connectDevice,t).then(function(e){200==e.body.status?(1==s.rulesData.length&&(s.pageNumber=s.pageNumber-1>0?s.pageNumber-1:1),s.getRulesList()):s.$message.error(e.body.message)})},submit:function(){var e=this;this.$refs.form.validate(function(s){if(!s)return e.$message.error(e.$t("accessSystem.qingtianxiebitianxinxi")),!1;var t=JSON.parse(a()(e.allWeeks));t.forEach(function(e,s){e.timeList.forEach(function(e,i){var a=e.timeQj[0],o=e.timeQj[1],n=""+(a%6*10<10?"0"+a%6*10:a%6*10),c=""+(o%6*10<10?"0"+o%6*10:o%6*10);t[s].timeList[i].timeQj=Math.floor(a/6)+":"+n+"-"+Math.floor(o/6)+":"+c})});["mon","tue","wed","thu","fri","sat","sun"].forEach(function(s,i){var a="";t[i].timeList.forEach(function(e){a=a?a+","+e.timeQj:e.timeQj}),e.form[s]=a}),e.isEdit?(console.log(e.form),e.$http.post(e.netAPI.accessRules.rulesModify,e.form).then(function(s){200==s.body.status?(e.$message.success(e.$t("accessSystem.xiugaichenggong")),e.getRulesList(),e.dialogAttrVisible=!1):e.$message.error(s.body.message)})):e.$http.post(e.netAPI.accessRules.rulesInsert,e.form).then(function(s){200==s.body.status?(e.$message.success(e.$t("accessSystem.tianjiachenggong")),e.getRulesList(),e.dialogAttrVisible=!1):e.$message.error(s.body.message)})})},getPassageWay:function(){var e=this;console.log(1),this.$http.get(this.netAPI.accessRules.passWayList).then(function(s){200==s.body.status&&(e.passWayList=s.body.data)})},opendetail:function(e){var s=this;this.$http.get(this.netAPI.accessRules.personTypeStatus).then(function(e){if(200==e.body.status)for(var t in s.peopleValidList=e.body.data,e.body.data)if(e.body.data.hasOwnProperty(t)){var i=e.body.data[t];0==i.personTypeStatus&&s.$set(i,"disabled",!0)}}),this.dialogAttrVisible=!0,e.id?(this.isEdit=!0,this.form.name=e.name,this.form.description=e.description,this.form.personType=e.personType,this.form.allow=a()(e.allow),console.log(this.form.allow),this.form.id=e.id,console.log(e.passageModes),null==e.passageModes&&(e.passageModes=[]),""==e.passageModes[0]&&(e.passageModes=[]),this.form.passageModes=e.passageModes,this.timeDataTransfer(e)):(this.isEdit=!1,this.form={name:"",description:"",personType:[],allow:"1",passageMode:"",passageModes:[]})},formatTimetip:function(e){var s=""+(e%6*10<10?"0"+e%6*10:e%6*10);return Math.floor(e/6)+":"+s},filterTimeInterVal:function(e){var s=e[0],t=e[1],i=""+(s%6*10<10?"0"+s%6*10:s%6*10),a=""+(t%6*10<10?"0"+t%6*10:t%6*10);return Math.floor(s/6)+":"+i+"-"+Math.floor(t/6)+":"+a},addTimeTip:function(e,s){this.allWeeks[e].timeList.length<4?this.$set(this.allWeeks[e].timeList,""+this.allWeeks[e].timeList.length,{timeQj:[0,144]}):this.$message.error(this.$t("accessSystem.meirishangxiansigeshijianduan"))},delTimeTip:function(e,s){this.allWeeks[e].timeList.splice(s,1)},tupTime:function(e,s){this.allWeeks[e].timeList=JSON.parse(a()(this.allWeeks[e-1].timeList))},handleSizeChange:function(e){this.pageSize=e,this.getRulesList()},handleCurrentChange:function(e){this.pageNumber=e,this.getRulesList()},getRulesList:function(){var e=this,s={pageNo:this.pageNumber,pageSize:this.pageSize,name:this.name,sort:"id",order:"asc"};this.$http.get(this.netAPI.accessRules.rulesList,{params:s}).then(function(s){if(200==s.body.status){for(var t in s.body.data)if(s.body.data.hasOwnProperty(t)){var i=s.body.data[t];for(var a in 0==i.allow?i.allowName=e.$t("accessSystem.fou"):i.allowName=e.$t("accessSystem.shi"),e.PeopleTypeList)if(e.PeopleTypeList.hasOwnProperty(a)){var o=e.PeopleTypeList[a];i.personType==o.categoryNo&&(i.personTypeName=o.categoryName)}for(var n in i.ruleDeviceVos)if(i.ruleDeviceVos.hasOwnProperty(n)){var c=i.ruleDeviceVos[n];i.ruleDeviceName?i.ruleDeviceName+=c.deviceName+"     ":i.ruleDeviceName=c.deviceName+"     "}for(var r in i.passageModes)if(i.passageModes.hasOwnProperty(r)){var l=i.passageModes[r];for(var u in e.passWayList)if(e.passWayList.hasOwnProperty(u)){var m=e.passWayList[u];m.categoryNo==l&&(i.passageModeName?i.passageModeName+=m.categoryName+"   , ":i.passageModeName=m.categoryName+",")}}}console.log(s.body.data),e.rulesData=s.body.data,e.total=s.body.total}else e.$message.error(s.body.message)})}}},l={render:function(){var e=this,s=e.$createElement,t=e._self._c||s;return t("div",{attrs:{id:"passRules"}},[t("el-dialog",{attrs:{title:e.$t("accessSystem.xiafashebei"),visible:e.deviceConnectVisible,width:"30%"},on:{"update:visible":function(s){e.deviceConnectVisible=s}}},[t("el-transfer",{attrs:{data:e.accessDeviceList},model:{value:e.accessDeviceIds,callback:function(s){e.accessDeviceIds=s},expression:"accessDeviceIds"}}),e._v(" "),t("div",{staticStyle:{"text-align":"center"}},[t("el-button",{attrs:{type:"primary"},on:{click:e.deviceClose}},[e._v(e._s(e.$t("accessSystem.queding")))]),e._v(" "),t("el-button",{on:{click:e.resetDevice}},[e._v(e._s(e.$t("accessSystem.quxiao")))])],1)],1),e._v(" "),t("div",{staticClass:"contantHeader"},[t("span",{staticClass:"title"},[e._v(e._s(e.$t("accessSystem.tongxingguize")))]),e._v(" "),t("div",{staticStyle:{display:"inline-block",float:"right","margin-right":"0.64rem"}},[t("el-button",{attrs:{round:""},on:{click:e.opendetail}},[e._v(e._s(e.$t("accessSystem.xinzeng")))])],1)]),e._v(" "),t("div",{staticClass:"contantTable"},[t("el-table",{staticStyle:{width:"100%"},attrs:{data:e.rulesData,"row-key":e.getRowKeys},on:{"selection-change":e.handleSelectionChange}},[t("el-table-column",{attrs:{prop:"name",label:e.$t("accessSystem.guizemingcheng")}}),e._v(" "),t("el-table-column",{attrs:{prop:"allowName",label:e.$t("accessSystem.shifouyunxutongxing")}}),e._v(" "),t("el-table-column",{attrs:{prop:"personTypeName",label:e.$t("accessSystem.renyuanleixing")}}),e._v(" "),t("el-table-column",{attrs:{prop:"passageModeName",label:e.$t("accessSystem.tongxingfangshi")}}),e._v(" "),t("el-table-column",{attrs:{label:e.$t("accessSystem.shengxiaoshebei")},scopedSlots:e._u([{key:"default",fn:function(s){return[t("el-popover",{attrs:{placement:"left-start",width:"70",trigger:"click"}},[t("span",{attrs:{slot:"reference"},slot:"reference"},[e._v(e._s(s.row.ruleDeviceName))]),e._v(" "),t("div",{staticStyle:{"max-height":"500px",overflow:"auto"}},e._l(s.row.ruleDeviceVos,function(i,a){return t("p",{key:a},[t("el-tag",{attrs:{color:"white",closable:"","disable-transitions":!1},on:{close:function(t){e.disconnectDevice(i,s.row.ruleDeviceVos)}}},[e._v("\n                "+e._s(i.deviceName)+"\n              ")])],1)}))])]}}])}),e._v(" "),t("el-table-column",{attrs:{label:e.$t("accessSystem.caozuo")},scopedSlots:e._u([{key:"default",fn:function(s){return[t("el-button",{attrs:{size:"mini",type:"text"},on:{click:function(t){e.opendetail(s.row)}}},[e._v(e._s(e.$t("accessSystem.bianji")))]),e._v(" "),1!=s.row.id&&2!=s.row.id&&3!=s.row.id?t("el-button",{attrs:{type:"text"},on:{click:function(t){e.deleteRule(s.row)}}},[e._v(e._s(e.$t("accessSystem.shanchu")))]):e._e(),e._v(" "),t("el-button",{staticClass:"btn",attrs:{type:"text",size:"mini"},on:{click:function(t){e.connectShow(s.row)}}},[e._v(e._s(e.$t("accessSystem.xiafashebei")))])]}}])})],1),e._v(" "),t("el-pagination",{staticClass:"pagination",attrs:{"current-page":e.pageNumber,"page-sizes":[5,10,20,40],"page-size":e.pageSize,layout:"total, sizes, prev, pager, next, jumper",total:e.total},on:{"size-change":e.handleSizeChange,"current-change":e.handleCurrentChange}})],1),e._v(" "),t("el-dialog",{staticClass:"dialogclass",attrs:{title:e.dialogType+e.$t("accessSystem.guize"),visible:e.dialogAttrVisible,width:"550px"},on:{"update:visible":function(s){e.dialogAttrVisible=s}}},[t("el-form",{ref:"form",attrs:{model:e.form,"label-width":"110px",rules:e.rules}},[t("el-form-item",{attrs:{label:e.$t("accessSystem.guizemingcheng"),prop:"name"}},[t("el-input",{attrs:{height:"20",maxlength:"20"},model:{value:e.form.name,callback:function(s){e.$set(e.form,"name",s)},expression:"form.name"}})],1),e._v(" "),t("el-form-item",{attrs:{label:e.$t("accessSystem.guizemiaoshu"),prop:"description"}},[t("el-input",{attrs:{type:"textarea",maxlength:"120"},model:{value:e.form.description,callback:function(s){e.$set(e.form,"description",s)},expression:"form.description"}})],1),e._v(" "),t("el-form-item",{attrs:{label:e.$t("accessSystem.renyuanleixing"),prop:"personType"}},[t("el-select",{attrs:{disabled:100010001003==e.form.personType||100010001002==e.form.personType||100010001001==e.form.personType},model:{value:e.form.personType,callback:function(s){e.$set(e.form,"personType",s)},expression:"form.personType"}},e._l(e.peopleValidList,function(e,s){return t("el-option",{key:s,attrs:{label:e.personTypeName,value:e.personType,disabled:e.disabled}})}))],1),e._v(" "),t("el-form-item",{attrs:{label:e.$t("accessSystem.shifouyunxutongxing"),prop:"passage"}},[t("el-radio-group",{model:{value:e.form.allow,callback:function(s){e.$set(e.form,"allow",s)},expression:"form.allow"}},[t("el-radio",{attrs:{label:"1"}},[e._v(e._s(e.$t("accessSystem.shi")))]),e._v(" "),t("el-radio",{attrs:{label:"0"}},[e._v(e._s(e.$t("accessSystem.fou")))])],1)],1),e._v(" "),1==e.form.allow?t("el-form-item",{attrs:{label:e.$t("accessSystem.tongxingfangshi"),prop:"passageWay"}},[t("el-checkbox-group",{model:{value:e.form.passageModes,callback:function(s){e.$set(e.form,"passageModes",s)},expression:"form.passageModes"}},e._l(e.passWayList,function(s,i){return t("el-col",{key:i,attrs:{span:8}},[t("div",{staticClass:"grid-content bg-purple"},[t("el-checkbox",{attrs:{label:s.categoryNo}},[e._v(e._s(s.categoryName))])],1)])}))],1):e._e(),e._v(" "),1==e.form.id&&1==e.form.allow||e.form.id>2&&1==e.form.allow||!e.form.id&&1==e.form.allow?t("div",{staticClass:"content-set"},e._l(e.allWeeks,function(s,i){return t("div",{key:i},[t("div",{staticClass:"week-word"},[e._v(e._s(s.name))]),e._v(" "),e._l(s.timeList,function(s,a){return t("div",{key:a,staticClass:"week-line"},[t("el-row",{attrs:{gutter:20}},[t("el-col",{attrs:{span:14}},[t("el-row",{attrs:{gutter:20}},[t("el-col",{attrs:{span:17}},[t("el-slider",{attrs:{range:"",label:"","format-tooltip":e.formatTimetip,max:144},model:{value:s.timeQj,callback:function(t){e.$set(s,"timeQj",t)},expression:"item.timeQj"}})],1),e._v(" "),t("el-col",{attrs:{span:7}},[t("div",{staticClass:"show-time"},[e._v(e._s(e.filterTimeInterVal(s.timeQj)))])])],1)],1),e._v(" "),t("el-col",{staticStyle:{"margin-top":"5px"},attrs:{span:7}},[0!=a?t("el-button",{attrs:{type:"primary",size:"mini"},on:{click:function(s){e.delTimeTip(i,a)}}},[e._v(e._s(e.$t("accessSystem.shanchu")))]):e._e(),e._v(" "),0===a?t("el-button",{attrs:{type:"primary",size:"mini"},on:{click:function(s){e.addTimeTip(i,a)}}},[e._v(e._s(e.$t("accessSystem.xinzeng")))]):e._e(),e._v(" "),0!==i&&0===a?t("el-button",{attrs:{type:"primary",size:"mini"},on:{click:function(s){e.tupTime(i,a)}}},[e._v(e._s(e.$t("accessSystem.tongshang")))]):e._e()],1)],1)],1)})],2)})):e._e(),e._v(" "),t("div",{staticClass:"btnGroup"},[t("el-button",{attrs:{type:"primary"},on:{click:function(s){e.submit()}}},[e._v(e._s(e.$t("accessSystem.queding")))]),e._v(" "),t("el-button",{attrs:{type:"primary"},on:{click:function(s){e.closeDialog()}}},[e._v(e._s(e.$t("accessSystem.quxiao")))])],1)],1)],1)],1)},staticRenderFns:[]};var u=t("VU/8")(r,l,!1,function(e){t("KtzV")},null,null);s.default=u.exports},KtzV:function(e,s){}});
//# sourceMappingURL=22.d7e3db80cce9bd7e3d24.js.map