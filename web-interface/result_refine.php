<!DOCTYPE html>
<?php
    include 'php_utils.php';
    $uid = $_GET['uid'];
    $history = get_latest_history('./Results/' . $uid . '_run/*_input_*.csv');
    $input_dim = get_dimension('./Results/' . $uid . '_run/' . $uid . '_input_' . intval($history) . '.csv');
?>
<html>
    <head>
        <link rel="stylesheet" type="text/css" href="ml_interface.css"> 
        <title> ML INTERFACE </title>
        <script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    </head>
    <body>
        <div id="title">
            <div id=title_text> ML INTERFACE </div>
        </div>
        <div id="inputs_div">
            <div id="plot_div"></div>
            <div id="panel_div">
                <br> <br> 
                <form id="refineForm" action="process_csv.php" method="post" enctype="multipart/form-data">
                    <input class='refine_form_fields' type='text' id='user_learning_rate' name='user_learning_rate' value='Learning rate' />
                    <input class='refine_form_fields' type='text' id='user_epoch' name='user_epoch' value='Epoch' />
                    <input class='refine_form_fields' type='submit' value='Refine' id="uploadFormSubmit" name='submit' />
                </form>
                <br> <br> <br> <br> <br> 
                <input type='text' readonly='readonly' id='weights_output' name='weights' />
            </div>
        </div>
    </body>
    <script>
        var uid = '<?php echo $_GET["uid"] ?>';
        var h = '<?php echo $_GET["history"] ?>';
        var dims = '<?php echo $input_dim ?>';
        var learning_rate = '<?php echo $_GET["learning_rate"] ?>';
        var epoch = '<?php echo $_GET["epoch"] ?>';
    </script>
    <script src="result_refine_js.js"></script>
</html>
