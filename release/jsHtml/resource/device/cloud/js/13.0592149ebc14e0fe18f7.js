webpackJsonp([13],{"6b87":function(t,e){},PfPb:function(t,e){},SDCx:function(t,e){},TPRB:function(t,e,i){"use strict";Object.defineProperty(e,"__esModule",{value:!0});var n={render:function(){var t=this.$createElement,e=this._self._c||t;return e("div",{attrs:{id:"header"}},[e("div",{staticClass:"navbar"},[this._m(0),this._v(" "),e("el-dropdown",{staticClass:"exist",on:{command:this.handleCommand}},[e("span",{staticClass:"el-dropdown-link"},[this._v("\n        下拉菜单"),e("i",{staticClass:"el-icon-arrow-down el-icon--right"})]),this._v(" "),e("el-dropdown-menu",{attrs:{slot:"dropdown"},slot:"dropdown"},[e("el-dropdown-item",{attrs:{command:"exist",divided:""}},[this._v("退出系统")])],1)],1)],1)])},staticRenderFns:[function(){var t=this.$createElement,e=this._self._c||t;return e("div",{staticClass:"title"},[e("span",[this._v("考拉智能监控管理系统")])])}]};var s={data:function(){return{isCollapse:!1,screenWidth:document.body.clientWidth}},mounted:function(){document.body.clientWidth<=1290?this.isCollapse=!0:this.isCollapse=!1;var t=this;window.onresize=function(){t.screenWidth=document.body.clientWidth}},methods:{initStyle:function(){this.screenWidth<=1290?this.isCollapse=!0:this.isCollapse=!1}},watch:{screenWidth:function(t){var e=this;this.timer||(this.screenWidth=t,this.timer=!0,setTimeout(function(){e.initStyle(),e.timer=!1},400))}}},o={render:function(){var t=this,e=t.$createElement,i=t._self._c||e;return i("div",{attrs:{id:"side"}},[i("el-menu",{staticClass:"el-menu-vertical-demo",attrs:{"default-active":t.$route.path,"background-color":"#2B333E","active-text-color":"#fff","text-color":"#A8B1BA",collapse:t.isCollapse,router:""}},[i("el-submenu",{attrs:{index:"1"}},[i("template",{slot:"title"},[i("i",{staticClass:"el-icon-location"}),t._v(" "),i("span",{attrs:{slot:"title"},slot:"title"},[t._v("智能安防管理")])]),t._v(" "),i("el-menu-item",{attrs:{index:"/monitoringSystem/monitoring_system/deviceManage"}},[t._v("设备管理")]),t._v(" "),i("el-menu-item",{attrs:{index:"/monitoringSystem/monitoring_system/faceControl"}},[t._v("人脸库管理")]),t._v(" "),i("el-menu-item",{attrs:{index:"/monitoringSystem/monitoring_system/eventManage"}},[t._v("事件管理")]),t._v(" "),i("el-menu-item",{attrs:{index:"/monitoringSystem/monitoring_system/monitoringRecord"}},[t._v("监控记录")]),t._v(" "),i("el-menu-item",{attrs:{index:"/monitoringSystem/monitoring_system/warning"}},[t._v("告警")])],2)],1)],1)},staticRenderFns:[]};var r={components:{klHeader:i("VU/8")({data:function(){return{}},methods:{handleCommand:function(t){"exist"===t&&"0"===this.$route.query.type?this.$router.push("/workbench/workbench"):this.$router.push("/monitoringLogin")}}},n,!1,function(t){i("6b87")},"data-v-3f0167c0",null).exports,klSide:i("VU/8")(s,o,!1,function(t){i("SDCx")},null,null).exports}},a={render:function(){var t=this.$createElement,e=this._self._c||t;return e("div",{attrs:{id:"main"}},[e("el-container",{staticClass:"content-box"},[e("el-header",[e("klHeader")],1),this._v(" "),e("el-container",{staticClass:"main-content"},[e("el-aside",{staticClass:"aside",attrs:{width:"212px"}},[e("klSide")],1),this._v(" "),e("el-main",[e("router-view")],1)],1)],1)],1)},staticRenderFns:[]};var l=i("VU/8")(r,a,!1,function(t){i("PfPb")},"data-v-68e32330",null);e.default=l.exports}});
//# sourceMappingURL=13.0592149ebc14e0fe18f7.js.map