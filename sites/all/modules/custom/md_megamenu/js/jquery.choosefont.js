(function(e){e.fn.choosefont=function(){return this.each(function(){function u(){_fontfamily=e("#"+n+"-fontfamily").val();_fontweight=e("#"+n+"-fontweight").val();_fontsize=e("#"+n+"-fontsize").val();_sizetype=e("#"+n+"-sizetype").val();_uppercase=e("#"+n+"-uppercase").val();_fontfamilydetail=Drupal.settings.font_vars[_fontfamily].CSS;_style="";if(_fontfamily!=0){_style+="font-family: "+_fontfamilydetail+";";if(_fontweight){_style+="font-weight: "+f(_fontweight).toLowerCase()+";"}}if(_fontsize){_style+="font-size: "+_fontsize+_sizetype+";"}if(_uppercase!="-"){_style+="text-transform: "+_uppercase+";"}e(r).val(_fontfamily+"|"+_fontweight+"|"+_fontsize+"|"+_sizetype+"|"+_uppercase+"|"+_style)}function a(t){_fontfamily=e("#"+t+"-fontfamily").val();_fontweight=e("#"+t+"-fontweight").val();_fontsize=e("#"+t+"-fontsize").val();_sizetype=e("#"+t+"-sizetype").val();_texttransform="none";if(e("#"+t+"-uppercase").is(":checked")){_texttransform="uppercase"}_color="#"+e("#"+t).next().find("input.form-colorpicker").val();_fontweightarr=_fontweight.split("");if(_fontweightarr[0]=="i"){_fontweightarr[0]="italic"}else{_fontweightarr[0]="normal"}_fontfamilydetail=Drupal.settings.font_vars[_fontfamily].CSS;e("#preview-"+t+" .tp-textdemo").css({"font-family":_fontfamilydetail,"font-weight":_fontweightarr[1]+"00","font-style":_fontweightarr[0],"font-size":_fontsize+_sizetype,"text-transform":_texttransform,color:_color})}function f(e,t){switch(e){case"n1":fontExpand="Thin";break;case"i1":fontExpand="Thin Italic";break;case"n2":fontExpand="Extra Light";break;case"i2":fontExpand="Extra Light Italic";break;case"n3":fontExpand="Light";break;case"i3":fontExpand="Light Italic";break;case"n4":fontExpand="Normal";break;case"i4":fontExpand="Italic";break;case"n5":fontExpand="Medium";break;case"i5":fontExpand="Medium Italic";break;case"n6":fontExpand="Semi Bold";break;case"i6":fontExpand="Semi Bold Italic";break;case"n7":fontExpand="Bold";break;case"i7":fontExpand="Bold Italic";break;case"n8":fontExpand="Extra Bold";break;case"i8":fontExpand="Extra Bold Italic";break;case"n9":fontExpand="Heavy";break;case"i9":fontExpand="Heavy Italic";break;default:fontExpand="undefined"}return fontExpand}var t=e(this),n=t.attr("id"),r="input[name="+n.replace(/-/g,"_")+"]",i="",s=1;t.find(".form-font").remove();data_arr=e(r).val().split("|");data_arr[0]=typeof data_arr[0]!=="undefined"?data_arr[0]:"0";if(!data_arr[0])data_arr[0]="0";data_arr[1]=typeof data_arr[1]!=="undefined"?data_arr[1]:"n4";data_arr[2]=typeof data_arr[2]!=="undefined"?data_arr[2]:"";data_arr[3]=typeof data_arr[3]!=="undefined"?data_arr[3]:"px";data_arr[4]=typeof data_arr[4]!=="undefined"?data_arr[4]:"0";if(!Drupal.settings.font_vars[data_arr[0]]){data_arr[0]="1"}var o=new Array("-","none","capitalize","lowercase","uppercase");html='<div class="form-font">';html+='<label for="'+n+'-fontfamily">Font</label>';html+='<select id="'+n+'-fontfamily" class="form-select2">';e.each(Drupal.settings.font_array,function(e,t){if(e==data_arr[0]){_select=' selected="selected"'}else{_select=""}html+="<option"+_select+' value="'+e+'">'+t+"</option>"});html+="</select>";html+='<div id="fontweight-'+n+'" class="form-font-weight"></div>';html+='<label for="'+n+'-fontsize">size:</label>';html+='<div class="form-font-size"><input type="text" maxlength="128" size="60" value="'+data_arr[2]+'" id="'+n+'-fontsize" class="font-size form-text" /></div>';html+='<select id="'+n+'-sizetype" class="form-select2">';if(data_arr[3]=="em"){html+='<option value="px">px</option><option selected="selected" value="em">em</option>'}else{html+='<option value="px">px</option><option value="em">em</option>'}html+="</select>";html+='<label for="'+n+'-uppercase">Text transform:</label>';html+='<select id="'+n+'-uppercase" class="form-select2">';e.each(o,function(e,t){if(e==data_arr[4]){_select=' selected="selected"'}else{_select=""}html+="<option"+_select+' value="'+t+'">'+t+"</option>"});html+="</select>";html+="</div>";t.prepend(html);i=data_arr[0];fontweight_html='<select id="'+n+'-fontweight" class="form-select3">';fontweight_arr=Drupal.settings.font_vars[i].Weight.split(",");e.each(fontweight_arr,function(t,n){optval=e.trim(n);if(optval==data_arr[1]){_select=' selected="selected"'}else{_select=""}fontweight_html+="<option"+_select+' value="'+optval+'">'+f(optval)+"</option>"});fontweight_html+="</select>";e("#fontweight-"+n).html(fontweight_html);if(!e("#previewbtn-"+n).length){t.addClass("withpreviewbtn").append('<div id="previewbtn-'+n+'" class="previewbtn"><a href="#">Preview</a></div>');t.append('<div id="preview-'+n+'" class="textpreview"><div class="tp-textdemo">Grumpy wizards make toxic brew for the evil Queen and Jack.</div><a href="#" class="tp-close">Close preview</a></div>');e("#previewbtn-"+n+" a").click(function(){a(n);e("#preview-"+n).show();e(this).text("Refresh").addClass("pbtn-refresh");return false});e("#preview-"+n+" .tp-close").click(function(){e(this).parent().hide();e("#previewbtn-"+n+" a").text("Preview").removeClass("pbtn-refresh");return false})}e("#"+n+"-fontfamily").change(function(){i=e(this).val();fontweight_html='<select id="'+n+'-fontweight" class="form-select3">';fontweight_arr=Drupal.settings.font_vars[i].Weight.split(",");e.each(fontweight_arr,function(t,n){optval=e.trim(n);fontweight_html+='<option value="'+optval+'">'+f(optval)+"</option>"});fontweight_html+="</select>";e("#fontweight-"+n).html(fontweight_html);s=1;u()});e("#"+n+"-fontweight").live("change",function(){u()});e("#"+n+"-fontsize").live("focusout",function(){u()});e("#"+n+"-sizetype,"+"#"+n+"-uppercase").change(function(){u()})})}})(jQuery)