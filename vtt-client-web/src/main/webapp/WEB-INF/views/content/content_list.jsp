<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>


<div class="row">
    <article class="col-xs-12 col-sm-12 col-md-12 col-lg-12">
        <div class="" id="wid-id-0" data-widget-editbutton="false">
            <div>
                <div class="jarviswidget-editbox">
                </div>
                <!-- widget content -->
                <div class="widget-body">
                    <%--<p>Adds borders to any table row within <code>&lt;table&gt;</code> by adding the <code>.table-bordered</code> with the base class</p>--%>
                    <div class="table-responsive">

                        <table class="table table-bordered">
                            <colgroup>
                                <col style="width:60px;">
                                <col style="width:60px;">
                                <col style="width:80px;">
                                <col style="width:80px;">
                                <col style="width:auto;">
                            </colgroup>
                            <thead>
                            <tr>
                                <th>순번</th>
                                <th>IDX</th>
                                <th>트랜스코딩</th>
                                <th>카탈로깅</th>
                                <th>제목</th>
                            </tr>
                            </thead>
                            <tbody id="content_assets">

                            <c:forEach var="item" items="${ listResponse.list }" varStatus="index" >
                            <tr>
                                <input type="hidden" name="idx" value="${ item.idx }" />
                                <td>${ listResponse.total - index.count + 1 }</td>
                                <td>${ item.idx }</td>
                                <td><c:import url="content_status.jsp"><c:param name="status" value="${item.transcodingstatus}"/></c:import></td>
                                <td><c:import url="content_status.jsp"><c:param name="status" value="${item.catalogstatus}"/></c:import></td>
                                <td class="text" >${ fn:replace(item.title, query.keyword, highlight ) }</td>
                            </tr>
                            </c:forEach>

                            </tbody>
                        </table>
                        <div class="dt-toolbar-footer">
                            <div class="col-sm-6 col-xs-12 hidden-xs">
                                <div class="dataTables_info" id="dt_basic_info" role="status" aria-live="polite"></div>
                            </div>
                            <div class="col-xs-12 col-sm-6">
                                <div class="dataTables_paginate paging_simple_numbers" id="dt_basic_paginate">
                                    <ul class="pagination">
                                        <%--<li class="paginate_button previous disabled" id="dt_basic_previous"><a href="#" aria-controls="dt_basic" data-dt-idx="0" tabindex="0">Previous</a></li>--%>
                                        <%--<li class="paginate_button active"><a href="#" aria-controls="dt_basic" data-dt-idx="1" tabindex="0">1</a></li>--%>
                                        <%--<li class="paginate_button "><a href="#" aria-controls="dt_basic" data-dt-idx="2" tabindex="0">2</a></li>--%>
                                        <%--<li class="paginate_button "><a href="#" aria-controls="dt_basic" data-dt-idx="3" tabindex="0">3</a></li>--%>
                                        <%--<li class="paginate_button "><a href="#" aria-controls="dt_basic" data-dt-idx="4" tabindex="0">4</a></li>--%>
                                        <%--<li class="paginate_button "><a href="#" aria-controls="dt_basic" data-dt-idx="5" tabindex="0">5</a></li>--%>
                                        <%--<li class="paginate_button disabled" id="dt_basic_ellipsis"><a href="#" aria-controls="dt_basic" data-dt-idx="6" tabindex="0">…</a></li>--%>
                                        <%--<li class="paginate_button "><a href="#" aria-controls="dt_basic" data-dt-idx="7" tabindex="0">10</a></li>--%>
                                        <%--<li class="paginate_button next" id="dt_basic_next"><a href="#" aria-controls="dt_basic" data-dt-idx="8" tabindex="0">Next</a></li>--%>
                                    </ul>
                                </div>
                            </div>
                        </div>
                    </div>

                </div>

            </div>

        </div>


    </article>

</div>

<script>

    var offset = parseInt("${listResponse.offset}");
    var limit = parseInt("${listResponse.limit}");
    var total_count = parseInt("${listResponse.total}");
    var current_page = parseInt((offset > 0 && limit > 0) ?  offset / limit : 0);
    var total_page = paginationTotalPage(total_count, limit);

    $("#dt_basic_info").text("Showing " + offset + " to " + (offset + limit) + " of " + total_count + " entrie");
    $(".pagination").bootpag({
        total: total_page,
        page: current_page + 1,
        maxVisible: 10,
        leaps: true,
        firstLastUse: true,
        first: '<i class="fa fa-angle-double-left" aria-hidden="true"></i>',
        prev: '<i class="fa fa-angle-left" aria-hidden="true"></i>',
        next: '<i class="fa fa-angle-right" aria-hidden="true"></i>',
        last: '<i class="fa fa-angle-double-right" aria-hidden="true"></i>'
    }).on("page", function(event, num) {
        params.offset = (num - 1) * params.limit;
    });


    $(".table tbody").selectable({
        filter: 'tr',
        distance: 1
    });

    $(".table tbody tr").on("click", "td:gt(0)", function (e) {
        var $T = $(this).parents("tr");
        var single = true;
        if (e.shiftKey) {
            var pos = $(".table tbody tr").index($T);
            var curr = $(".table tbody tr").index($(".ui-selected"));
            if (pos != curr) {
                $('.table tbody tr').removeClass('ui-selected');
                $('.table tbody tr').slice(Math.min(pos, curr), 1 + Math.max(pos, curr)).addClass('ui-selected');
                single = false;
            }
        }
        if (e.ctrlKey) {
            $T.addClass('ui-selected');
            single = false;
        }
        if (single) {
            $('.table tbody tr').removeClass('ui-selected');
            $T.addClass('ui-selected');
        }
    });

    $(".table tbody tr").on("dblclick", "td:gt(0)", function (e) {
        $("#btn_storyboard").click();
    });


</script>
