_jsload2&&_jsload2('groundoverlay', 'z.extend(vc.prototype,{ib:function(){if(this.map){this.V||this.Ej();var a=this.map.fa();if(a<this.z.Bs||a>this.z.As)this.U();else{var b=this.z.bb.Ve(),c=this.z.bb.Rf(),a=this.map.Xe(b).x,e=this.map.Xe(c).y,c=this.map.Xe(c).x,b=this.map.Xe(b).y;this.V.style.left=a+"px";this.V.style.top=e+"px";this.V.style.width=c-a+"px";this.V.style.height=b-e+"px";this.show()}}},draw:function(){this.ib()},Ej:function(){if(!this.V){this.V=document.createElement("div");var a=this.V.style;a.cssText="position: absolute; -moz-user-select: none; overflow: hidden;"; a.zIndex=mc.Ck(this.z.bb.Ve().lat);this.z.opacity&&(a.opacity=this.z.opacity,a.filter="alpha(opacity="+100*parseInt(this.z.opacity,10)+")");this.Og=document.createElement("img");this.Og.style.width="100%";this.z.X_&&(this.Og.style.height="100%");this.V.appendChild(this.Og);var b=this;this.Og.onload=function(){b.P1=b.Og.naturalWidth||b.Og.width;b.O1=b.Og.naturalHeight||b.Og.height};this.z.Gm&&(this.Og.src=this.z.Gm);this.map.Tf().KE.appendChild(this.V);b=this;z.M(this.V,"click",function(a){b.dispatchEvent(new P("click"), a)});z.M(this.V,"dblclick",function(a){b.dispatchEvent(new P("dblclick"),a)})}},s_:function(a){this.z.bb=a;this.map&&this.ib();this.V.style.zIndex=mc.Ck(this.z.bb.Ve().lat)},ve:function(){return this.z.bb},HF:function(a){this.z.opacity=a;this.V.style.opacity=a;this.V.style.filter="alpha(opacity="+100*parseInt(a,10)+")"},XX:function(){return this.z.opacity},x_:function(a){this.z.Gm=a;this.Og&&(this.Og.src=a)},KX:function(){return this.z.Gm},p5:function(a){this.z.Bs=a},r3:function(){return this.z.Bs}, o5:function(a){this.z.As=a},q3:function(){return this.z.As}});vc.prototype.setBounds=vc.prototype.s_;vc.prototype.getBounds=vc.prototype.ve;vc.prototype.setOpacity=vc.prototype.HF;vc.prototype.getOpacity=vc.prototype.XX;vc.prototype.setImageURL=vc.prototype.x_;vc.prototype.getImageURL=vc.prototype.KX;vc.prototype.setDispalyOnMinLevel=vc.prototype.n5;vc.prototype.setDispalyOnMaxLevel=vc.prototype.m5; ');