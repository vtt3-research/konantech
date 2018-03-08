! function(a) {
    var shoid_befor;
    var sel_item;
    a.fn.SuperBox = function(b) {
        var c = a('<div class="superbox-show"></div>'),
            g = a('<div class="superbox_image" ></div>'),
            i = a('<div class="superbox_image_div" ></div>'),
            h = a('<img src="" class="thumbnail img-responsive">'),
            d = a('<div id="imgInfoBox" class="superbox-imageinfo inline-block"> <textarea class="shot_description" onkeyup="setTextShotDescription($(this))"></textarea> </div>'),
            e = a('');
        c.append(g.append(i.append(h))).append(d).append(e);
        a(".superbox-imageinfo");
        return this.each(function() {
            a(".superbox-list").click(function() {
                $this= a(this);

                var shoid = $this.find("input[name=shotid]").val();

                if($(".superbox-list").hasClass("active")){ //close
                    if(shoid_befor == shoid || undefined == shoid){
                        $this.find("input[name=annotationdescription]").val($(".shot_description").val());
                    }else{
                        sel_item.find("input[name=annotationdescription]").val($(".shot_description").val());
                        $(".shot_description").val($this.find("input[name=annotationdescription]").val());
                    }
                }

                sel_item = $this;
                shoid_befor = shoid;

                var b = $this.find(".superbox-img"),
                    e = b.data("img"),
                    f = b.attr("alt") || "No description",
                    g = e;

                h.attr("src", e), a(".superbox-list").removeClass("active"), $this.addClass("active"), d.find("em").text(g), d.find(".superbox-img-description").text(f), 0 == a(".superbox-current-img").css("opacity") && a(".superbox-current-img").animate({
                    "opacity": 1
                }), a(this).next().hasClass("superbox-show") ? (c.is(":visible") && a(".superbox-list").removeClass("active"), c.toggle()) : (c.insertAfter(this).css("display", "block"), $this.addClass("active")), a("html, body").animate({
                    "scrollTop": c.position().top - b.width()
                }, "medium");

                if($(".shot_description") != undefined){
                    $(".shot_description").val($this.find("input[name=annotationdescription]").val());
                }


            }), a(".superbox").on("click", ".superbox-close", function() {
                if($(".shot_description").val() != $this.find("input[name=annotationdescription]").val()){
                    var result = confirm("입력된 값을 취소 하시겠습니까?");
                    if(!result){
                        return;
                    }
                }
                a(".superbox-list").removeClass("active"), a(".superbox-current-img").animate({
                    "opacity": 0
                }, 200, function() {
                    a(".superbox-show").slideUp()
                })
            })
        })
    }
}(jQuery);