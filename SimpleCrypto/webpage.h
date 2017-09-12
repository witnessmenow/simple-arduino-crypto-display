R"(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
  </head>
  <body>
    <div class="container-fluid">
      <div class="row">
        <h3>Holdings</h3>
        <div id="holdingList" class="list-group col-md-6" style="padding: 1em">
          <div class="list-group-item">
            ethereum
            <span class="pull-right">
                <span class="btn btn-xs btn-default" onclick="makeAjaxCall('delete?holding=" + holdingId + ");">
                    <span class="glyphicon glyphicon-remove" aria-hidden="true"></span>
                </span>
            </span>
          </div>
        </div>
      </div>
      <div class="row">
        <form id="addHoldingForm" action="add" method="GET" class="form-inline" style="padding: 1em">
          <div class="form-group">
            <label for="holdingIdField" style="padding: 0.5em">Enter holding name</label>
            <input type="text" class="form-control" name="holding" placeholder="E.G. ethereum">
          </div>
          <button type="submit" class="btn btn-default">Add Holding</button>
        </form>
      </div>
    </div>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <script> 
      function makeAjaxCall(url){$.ajax({"url": url})}

      function addHolding(holdingId){
        var holdingHtml = '<div class="list-group-item">' + 
                            holdingId +
                            '<span class="pull-right">' +
                              '<span class="btn btn-xs btn-default" onclick="makeAjaxCall(\'delete?holding=' + holdingId + '\');">' +
                                '<span class="glyphicon glyphicon-remove" aria-hidden="true"></span>' +
                              '</span>' +
                            '</span>' +
                          '</div>';
                            
                        
        $('#holdingList').append(holdingHtml);
      }

      var frm = $('#addHoldingForm');

      frm.submit(function() {
        $.ajax({
          type: $(this).attr('method'),
          url:  $(this).attr('action'),
          data: $(this).serialize(),
          success: function (data) {
              console.log('Submission was successful.');
              console.log(data);
          },
          error: function (data) {
              console.log('An error occurred.');
              console.log(data);
          },
        });

        return false;
      });
    </script>
  </body>
</html>
)"

