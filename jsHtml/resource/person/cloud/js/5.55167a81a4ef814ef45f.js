webpackJsonp([5],{"4rO5":function(e,t){},"849G":function(e,t){},WsNT:function(e,t,i){"use strict";var o=i("BO1k"),n=i.n(o),s=i("2hMI"),a={name:"isShow",props:{peopleTypeInfo:{type:Array,default:[]},peopleBaseInfo:{type:Object,default:{}},isEditPersonType:{type:Boolean,default:!1},dialogAddPeopleType:{type:Boolean,default:!1}},data:function(){return{netAPI:"",Form:{peopleType:this.peopleBaseInfo.peopleTypeName,attribute:[this.$t("personnelType.name"),this.$t("personnelType.code")],checkList:[this.$t("personnelType.name"),this.$t("personnelType.code")]},attributesArr:[],rules:{peopleType:[{trigger:"blur"}]},filterMethod:function(e,t){return t.description.indexOf(e)>-1},res:[],showGroup:!1}},methods:{handleClose:function(){arguments.length>0&&void 0!==arguments[0]&&arguments[0];this.$emit("dialogClosePeopleType")},submitForm:function(e){var t=this;this.$refs.ruleForm.validate(function(e){if(!e)return console.log("error submit!!"),!1;t.submit()})},submit:function(){var e=this;if(""!==this.Form.peopleType){var t={name:this.Form.peopleType,children:[],isGroup:this.showGroup},i=!0,o=!1,s=void 0;try{for(var a,l=n()(this.res);!(i=(a=l.next()).done);i=!0){var r=a.value,p=!0,c=!1,d=void 0;try{for(var u,h=n()(this.Form.attribute);!(p=(u=h.next()).done);p=!0){var f=u.value;r.description==f&&t.children.push({description:f,name:r.name,isGroup:this.showGroup})}}catch(e){c=!0,d=e}finally{try{!p&&h.return&&h.return()}finally{if(c)throw d}}}}catch(e){o=!0,s=e}finally{try{!i&&l.return&&l.return()}finally{if(o)throw s}}t.children.forEach(function(t,i){var o=!1;e.Form.checkList.forEach(function(e,i){t.description===e&&(o=!0)}),t.note=o?{required:"0"}:{required:"1"}}),t.children.length>20?this.$message.success(this.$t("personnelType.numFiledTips")):this.isEditPersonType?(t.no=this.peopleBaseInfo.categoryNo,t.id=this.peopleBaseInfo.id,this.$http.post(this.netAPI.typeModify,t).then(function(t){200==t.body.status&&(e.$message.success(e.$t("common.modifySuccess")),e.$emit("dialogClosePeopleType"),e.handleClose("refresh"))})):this.$http.post(this.netAPI.insertPeopleType,t).then(function(t){200==t.body.status&&(e.$message.success(e.$t("common.addSuccess")),e.$emit("dialogClosePeopleType"),e.initData())})}else this.$message.warning("please enter a type name")},getAttributes:function(){var e=this;this.$http.get(this.netAPI.findPeopleAttribute,{params:{categoryNo:10001001}}).then(function(t){200==t.body.status&&(e.attributesArr.length=0,e.attributesArr=t.body.data,e.attributesArr.forEach(function(t,i){"100010011001"!==t.no&&"100010011002"!==t.no||(e.attributesArr[i].disabled=!0)}),e.res=t.body.data)})},initData:function(){this.Form={peopleType:this.peopleBaseInfo.peopleTypeName,attribute:[this.$t("personnelType.name"),this.$t("personnelType.code")],checkList:[this.$t("personnelType.name"),this.$t("personnelType.code")]}},trim:function(e){return e.replace(/(^\s*)|(\s*$)/g,"")},getPersonFieldName:function(){var e=this;this.initData(),this.peopleTypeInfo.forEach(function(t){t.description!==e.$t("personnelType.name")&&t.description!==e.$t("personnelType.code")&&(e.Form.attribute.push(t.description),"0"===t.note.required&&e.Form.checkList.push(t.description))})},distGroup:function(){var e=this.peopleBaseInfo.categoryValue;JSON.parse(e).hasOwnProperty("group")?this.showGroup=!0:this.showGroup=!1}},created:function(){this.netAPI=Object(s.a)().get("people_manage"),this.getAttributes(),this.isEditPersonType&&(this.getPersonFieldName(),this.distGroup())},watch:{dialogAddPeopleType:function(e){!0===e&&(this.getAttributes(),this.distGroup(),this.getPersonFieldName())},peopleBaseInfo:{handler:function(e){this.distGroup(),this.Form.peopleType=e.peopleTypeName},deep:!0}}},l={render:function(){var e=this,t=e.$createElement,i=e._self._c||t;return i("div",{staticClass:"new-people-type"},[i("el-form",{ref:"ruleForm",attrs:{model:e.Form,rules:e.rules,"label-width":"100px"}},[i("el-form-item",{attrs:{label:e.$t("personnelType.typeName"),prop:"peopleType"}},[i("el-input",{attrs:{placeholder:e.$t("personnelType.pleaseEnterTypeName"),disabled:e.isEditPersonType,clearable:""},model:{value:e.Form.peopleType,callback:function(t){e.$set(e.Form,"peopleType","string"==typeof t?t.trim():t)},expression:"Form.peopleType"}})],1),e._v(" "),i("el-form-item",{attrs:{label:e.$t("personnelType.infField"),prop:"infoAttribute"}},[i("div",[i("el-checkbox-group",{model:{value:e.Form.checkList,callback:function(t){e.$set(e.Form,"checkList",t)},expression:"Form.checkList"}},e._l(e.Form.attribute,function(t,o){return i("div",{key:o},[t==e.$t("personnelType.name")||t==e.$t("personnelType.code")?i("el-checkbox",{key:o,attrs:{label:t,disabled:""}}):i("el-checkbox",{key:o,attrs:{label:t}})],1)}))],1),e._v(" "),i("div",[i("span",{staticStyle:{float:"left"}},[e._v(e._s(e.$t("personnelType.explain")))]),e._v(" "),i("span",{staticStyle:{float:"right"}},[i("el-popover",{attrs:{placement:"right",width:"500",trigger:"click"}},[[i("el-transfer",{attrs:{props:{key:"description",label:"description"},data:e.attributesArr,titles:[e.$t("personnelType.unchecked"),e.$t("personnelType.selection")],filterable:"","filter-method":e.filterMethod,"filter-placeholder":e.$t("personnelType.inputField")},model:{value:e.Form.attribute,callback:function(t){e.$set(e.Form,"attribute",t)},expression:"Form.attribute"}})],e._v(" "),i("el-button",{attrs:{slot:"reference",icon:"el-icon-edit",circle:""},slot:"reference"})],2)],1)])]),e._v(" "),i("el-form-item",{attrs:{label:e.$t("personnelType.group"),prop:"global"}},[i("el-switch",{attrs:{"active-text":e.$t("personnelType.yes"),"inactive-text":e.$t("personnelType.no")},model:{value:e.showGroup,callback:function(t){e.showGroup=t},expression:"showGroup"}})],1)],1),e._v(" "),i("div",{staticClass:"dialog-footer",attrs:{slot:"footer"},slot:"footer"},[i("el-button",{on:{click:e.handleClose}},[e._v(e._s(e.$t("common.cancel")))]),e._v(" "),i("el-button",{attrs:{type:"primary"},on:{click:e.submit}},[e._v(e._s(e.$t("common.ok")))])],1)],1)},staticRenderFns:[]};var r=i("VU/8")(a,l,!1,function(e){i("849G")},null,null);t.a=r.exports},apLG:function(e,t,i){"use strict";Object.defineProperty(t,"__esModule",{value:!0});var o=i("4syG"),n=i("WsNT"),s=i("viLm"),a=i("2hMI"),l={name:"peopleType",components:{separator:o.a,newPeopleTypeDialog:n.a,peopleAttrConfigDialog:s.a},data:function(){return{netApi:"",currentPage:1,pageSize:10,total:6,tableData:[],showData:[],dialogPeopleTitle:"新增人员类型",dialogAddPeopleType:!1,dialogFixField:!1,tableBoxLoading:!1,peopleTypeInfo:[],newTableData:[],peopleBaseInfo:{peopleTypeName:"",peopleTypeNo:"",categoryValue:""},isEditPersonType:!1,deleteform:{no:""}}},watch:{currentPage:function(e){this.changeShowData()},pageSize:function(){this.changeShowData()}},created:function(){this.netAPI=Object(a.a)().get("people_manage"),this.getPeopleCotegory()},methods:{changeShowData:function(){this.showData=this.tableData.slice((this.currentPage-1)*this.pageSize,(this.currentPage-1)*this.pageSize+this.pageSize)},handleSizeChange:function(e){},handleCurrentChange:function(e){this.currentPage=e},filterName:function(e){if(console.log(e),"100010001001"==e.no||"100010001002"==e.no||"100010001003"==e.no)return!0},editRow:function(e){console.log(e),this.dialogAddPeopleType=!0,this.peopleTypeInfo=e.row.children,this.peopleBaseInfo.id=e.row.id,this.peopleBaseInfo.peopleTypeName=e.row.name,this.peopleBaseInfo.categoryNo=e.row.no,this.peopleBaseInfo.categoryValue=e.row.value,this.isEditPersonType=!0,this.dialogPeopleTitle=this.$t("personnelType.edit")},deleteRow:function(e){var t=this;this.deleteform.no=e.no,this.deleteform.name=e.name,this.$http.post(this.netAPI.typeRemove,[this.deleteform]).then(function(e){200==e.body.status&&(t.initParam(),t.$message.success(t.$t("delete.success")),t.getPeopleCotegory())},function(e){t.$message.warning(""+e)})},initParam:function(){this.currentPage=1},getPeopleCotegory:function(){var e=this;this.tableBoxLoading=!0,this.$http.get(this.netAPI.findPeopleCategory).then(function(t){200==t.body.status?(e.total=t.data.data.length,e.tableData=t.data.data,e.changeShowData(),e.tableBoxLoading=!1):e.$message.warning(""+e.$t("personnelType.getPersonFailed")+t.body.message)})},addPeopleType:function(){this.dialogAddPeopleType=!0,this.isEditPersonType=!1,this.peopleBaseInfo.peopleTypeName="",this.dialogPeopleTitle=this.$t("personnelType.addPersonType")},fixPeopleField:function(){this.dialogFixField=!0},dialogClosePeopleType:function(){this.dialogAddPeopleType=!1,this.getPeopleCotegory()},closeDialogFixField:function(){this.dialogFixField=!1}}},r={render:function(){var e=this,t=e.$createElement,i=e._self._c||t;return i("div",{staticClass:"people-type"},[i("div",{staticClass:"people-type-style"},[i("separator"),e._v(" "),i("div",{staticClass:"type-content"},[i("div",{staticClass:"top-box"},[i("i",{staticClass:"blue"}),e._v(" "),i("div",{staticClass:"title"},[i("span",[e._v(e._s(e.$t("personnelType.title")))])]),e._v(" "),i("div",{staticClass:"type-content-fix"},[i("el-button",{attrs:{type:"primary"},on:{click:e.addPeopleType}},[e._v(e._s(e.$t("personnelType.add")))]),e._v(" "),i("el-button",{attrs:{type:"primary"},on:{click:e.fixPeopleField}},[e._v(e._s(e.$t("personnelType.field")))])],1)]),e._v(" "),i("div",{staticClass:"show-table"},[i("el-table",{directives:[{name:"loading",rawName:"v-loading",value:e.tableBoxLoading,expression:"tableBoxLoading"}],attrs:{data:e.showData,"header-cell-style":{background:"rgba(245,248,252,1)"},height:"631"}},[i("el-table-column",{attrs:{label:e.$t("allocation.index"),type:"index",width:"150",align:"center"}}),e._v(" "),i("el-table-column",{attrs:{prop:"name",label:e.$t("personnelType.title"),align:"center"}}),e._v(" "),i("el-table-column",{attrs:{label:e.$t("common.operation"),align:"center"},scopedSlots:e._u([{key:"default",fn:function(t){return[i("el-button",{attrs:{type:"text",size:"small"},nativeOn:{click:function(i){i.preventDefault(),e.editRow(t)}}},[e._v("\n                  "+e._s(e.$t("common.edit"))+"\n                ")]),e._v(" "),i("el-button",{attrs:{disabled:e.filterName(t.row),type:"text",size:"small"},nativeOn:{click:function(i){i.preventDefault(),e.deleteRow(t.row)}}},[e._v("\n                  "+e._s(e.$t("common.delete"))+"\n                ")])]}}])})],1),e._v(" "),i("div",{staticClass:"pagination-box"},[i("el-pagination",{attrs:{"current-page":e.currentPage,"page-sizes":[5,10,20],"page-size":e.pageSize,layout:"total, sizes, prev, pager, next, jumper",total:e.total},on:{"size-change":e.handleSizeChange,"current-change":e.handleCurrentChange,"update:pageSize":function(t){e.pageSize=t}}})],1)],1)])],1),e._v(" "),i("el-dialog",{attrs:{title:e.dialogPeopleTitle,visible:e.dialogAddPeopleType,width:"30%"},on:{"update:visible":function(t){e.dialogAddPeopleType=t}}},[i("new-people-type-dialog",{attrs:{isEditPersonType:e.isEditPersonType,dialogAddPeopleType:e.dialogAddPeopleType,peopleTypeInfo:e.peopleTypeInfo,peopleBaseInfo:e.peopleBaseInfo},on:{dialogClosePeopleType:e.dialogClosePeopleType}})],1),e._v(" "),i("el-dialog",{attrs:{title:e.$t("personnelType.field"),visible:e.dialogFixField,width:"35%"},on:{"update:visible":function(t){e.dialogFixField=t}}},[i("people-attr-config-dialog",{attrs:{dialogFixField:e.dialogFixField},on:{closeDialogFixField:e.closeDialogFixField}})],1)],1)},staticRenderFns:[]};var p=i("VU/8")(l,r,!1,function(e){i("lWNI")},null,null);t.default=p.exports},lWNI:function(e,t){},viLm:function(e,t,i){"use strict";var o=i("BO1k"),n=i.n(o),s=i("2hMI"),a={name:"isShow",props:["isShow","dialogFixField"],data:function(){return{attrs:[],input:"",netAPI:"",inputVisible:!1}},created:function(){this.netAPI=Object(s.a)().get("people_manage"),this.getData()},methods:{handleClose:function(e){this.$emit("closeDialogFixField")},handleDeleteTag:function(e){var t=this;console.log(e);var i=this.$loading({lock:!0,text:"loading",spinner:"el-icon-loading",background:"rgba(0, 0, 0, 0.1)"});this.$http.post(this.netAPI.deletePeopleAttribute,[{no:e.no}]).then(function(e){i.close(),200===e.data.status&&(t.$message.success("success"),t.getData())})},submit:function(){var e=this;this.inputVisible=!1;if(this.input){if(!/^[A-Za-z]+$/.test(this.input))return this.$message.warning(this.$t("peopleManageList.fieldTips")),this.input="",!1;var t=!0,i=!1,o=void 0;try{for(var s,a=n()(this.attrs);!(t=(s=a.next()).done);t=!0){var l=s.value;if(this.input===l.description)return this.input="",void this.$message.warning(this.$t("peopleManageList.fieldDupTips"))}}catch(e){i=!0,o=e}finally{try{!t&&a.return&&a.return()}finally{if(i)throw o}}var r=this.$loading({lock:!0,text:"loading",spinner:"el-icon-loading",background:"rgba(0, 0, 0, 0.1)"});this.$http.post(this.netAPI.insertPeopleAttribute,{description:this.input}).then(function(t){200==t.body.status?(r.close(),e.$message.success("success!"),e.input="",e.getData()):r.close()})}},showInput:function(){var e=this;this.inputVisible=!0,this.$nextTick(function(t){e.$refs.saveTagInput.$refs.input.focus()})},getData:function(){var e=this;this.$http.get(this.netAPI.findPeopleAttribute,{params:{categoryNo:10001001}}).then(function(t){200==t.body.status&&(e.attrs=t.body.data)})},trim:function(e){return e.replace(/(^\s*)|(\s*$)/g,"")}},watch:{dialogFixField:function(){this.input="",this.getData()}}},l={render:function(){var e=this,t=e.$createElement,i=e._self._c||t;return i("div",{staticClass:"people-attr-config-dialog"},[i("div",[e._l(e.attrs,function(t,o){return i("el-tag",{key:o,staticClass:"tag-style",attrs:{closable:"",type:""},on:{close:function(i){e.handleDeleteTag(t)}}},[e._v("\r\n        "+e._s(t.description)+"\r\n      ")])}),e._v(" "),e.inputVisible?i("el-input",{ref:"saveTagInput",staticClass:"input-new-tag",attrs:{size:"small"},on:{blur:e.submit},model:{value:e.input,callback:function(t){e.input=t},expression:"input"}}):i("el-button",{staticClass:"button-new-tag",attrs:{size:"small"},on:{click:e.showInput}},[e._v("+ "+e._s(e.$t("personnelType.new")))]),e._v(" "),i("div",{staticClass:"dialog-footer",attrs:{slot:"footer"},slot:"footer"},[i("el-button",{attrs:{type:"primary"},on:{click:e.handleClose}},[e._v(e._s(e.$t("common.ok")))])],1)],2)])},staticRenderFns:[]};var r=i("VU/8")(a,l,!1,function(e){i("4rO5")},null,null);t.a=r.exports}});
//# sourceMappingURL=5.55167a81a4ef814ef45f.js.map