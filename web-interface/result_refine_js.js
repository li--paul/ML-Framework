
var uid = '<?php $_GET["uid"] ?>';
var history = '<?php $_GET["history"] ?>';
var check_for_dumps_interval;
var update_plot_interval;

function check_for_dumps() {
    /* If the dump files are generated;
     * then we are ready to read the dumps
     */
    var parameter_map = {
                            "history" : history,
                            "uid"     : uid
                        };
    $.ajax({
        url: "dump_file_status.php",
        type: "POST",
        data: parameter_map,
        success: function(str) {
            if(str.localeCompare("2") == 0) {
                /* Move to the results page */
                var history_val = parseInt(history);
                window.location = 'result_refine.php?' + 'uid=' + uniq_id + '&history=' + (history_val + 1).toString() + '&lr=' + learning_rate.toString() + '&epoch=' + epoch.toString();
            } else if(str.localeCompare("0")){
                clearInterval(check_for_dumps);
                update_plot_interval = setInterval(update_plot, 1500);
            }
        }
    });

} 

$(document).ready(
    function interval_kickoff() {
        check_for_dumps_interval = setInterval(check_for_dumps, 1500);
    }

);

$("#refineForm").submit(function(event) {
    event.preventDefault();

    var learning_rate = parseFloat(document.getElementById('user_learning_rate').value);
    var epoch = parseFloat(document.getElementById('user_epoch').value);

    if(isNaN(learning_rate) || learning_rate < 0) {
        /* Use default learning rate */
        learning_rate = 0;
    }

    if(isNaN(epoch) || epoch <= 0) {
        /* Using default value */
        epoch = -1;
    }

    var parameter_map = {
                        "uid"             : uid,
                        "learning_rate"   : learning_rate,
                        "epoch"           : epoch
                        };

    var history_val = parseInt(history);
    $.ajax({
        url: "process_csv.php",
        type: "POST",
        data: parameter_map,
        success: function(str) {
            if(str.localeCompare("0") == 0) {
                /* Move to the results page */
                window.location = 'result_refine.php?' + 'uid=' + uniq_id + '&history=' + (history_val + 1).toString() + '&lr=' + learning_rate.toString() + '&epoch=' + epoch.toString();
            } else {
                alert(str + '... Aborting');
            }
        }
    });
});
