
var check_for_dumps_interval;
var update_plot_interval;

function draw_plot(data, weights, cost) {
    var lines = data.split("\n");
    var x_pts = [];
    var y_pts = [];
    for(var line_iter = 0; line_iter < lines.length; line_iter++) {
        if (lines[line_iter].localeCompare("") == 0) {
            continue;
        }
        var words = lines[line_iter].split(",");
        var cleaned_words = [];
        for(var word_iter = 0; word_iter < words.length; word_iter++) {
            var word = words[word_iter];
            if(word.localeCompare("") != 0){
                cleaned_words.push(word);
            } 
        }

        if(cleaned_words.length != 2) {
            alert("Trying to plot data of dim > 2 or < 2 | length " + cleaned_words.length.toString());
            return;
        }

        x_pts.push(cleaned_words[0]);
        y_pts.push(cleaned_words[1]);
    }

    var scatter_data =  {
                            x: x_pts,
                            y: y_pts,
                            type: 'scatter',
                            mode: 'markers'
                        };


    var data = [scatter_data];
    var layout = {  
                    title: 'scatter-plot',
                    xaxis: {
                                title: 'x-axis',
                                showgrid: true
                            },
                    yaxis: {
                                title: 'y-axis',
                                showgrid: true
                            },
                };
    
    Plotly.newPlot(plot_div, data, layout);
}

function update_plot(input_path, weights_path) {
    var parameter_map_input = { 'fpath'   : input_path };
    var parameter_map_weights = { 'fpath' : weights_path };

    var input_points = "";
    var weights = "";
    var cost = "";

    /* Get input points */
    $.ajax({
        url: "get_file_contents.php",
        type: "POST",
        data: parameter_map_input,
        success: function(str) {
            if(str.localeCompare("-1") == 0) {
                alert("get_input_points fail");
                return;
            } else {
                input_points = str;
                /* Get weights */
                $.ajax({
                    url: "get_file_contents.php",
                    type: "POST",
                    data: parameter_map_weights,
                    success: function(str) {
                        if(str.localeCompare("-1") == 0) {
                            weights = "";
                            cost = "";
                        } else {
                            var str_parts = str.split(" - ");
                            weights = str_parts[0];
                            cost = str_parts[1];
                        }
                        draw_plot(input_points, weights, cost);
                    }
                });
            }
        }
    });
}

function check_for_dumps() {
    /* If the dump files are generated;
     * then we are ready to read the dumps
     */
    var parameter_map = {
                            "history" : h,
                            "uid"     : uid
                        };
    $.ajax({
        url: "dump_file_status.php",
        type: "POST",
        data: parameter_map,
        success: function(str) {
            if(str.localeCompare("2") == 0) {
                /* Move to the results page */
                var history_val = parseInt(h);
                window.location = 'result_refine.php?' + 'uid=' + uniq_id + '&history=' + (history_val + 1).toString() + '&lr=' + learning_rate.toString() + '&epoch=' + epoch.toString();
            } else if(str.localeCompare("0")){
                clearInterval(check_for_dumps);
                update_plot_interval = setInterval(update_plot, 1500);
            }
        }
    });

} 

$(document).ready(
    function process_kickoff() {
        if(dims == 2) {
            update_plot('./Results/' + uid + '_run/' + uid + '_input_' + h + '.csv',
                        './Results/' + uid + '_run/' + uid + '_weights_' + h + '.csv');
        }
        return;
        //check_for_dumps_interval = setInterval(check_for_dumps, 1500);
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

    var history_val = parseInt(h);
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
