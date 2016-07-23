
var check_for_dumps_interval;
/* Default line */
var line_data = {
                x: [0],
                y: [0],
                mode: 'lines+markers'
            };

/*
 * Plots the data points.
 * Arguments : 
 *  data - Path to the data points csv file.
 *  weights - Path to the weights csv file. ( if
 *            the weights csv file is not available
 *            dont panic. Dont plot it)
 *  cost - Cost associated with the weights.
 */
function draw_plot(data, weights, cost) {

    var lines = data.split("\n");
    var x_pts = [];
    var y_pts = [];
    var line_x_pts = [];
    var line_y_pts = [];

    var plot_extent_max_x = Number.MIN_VALUE;
    var plot_extent_max_y = Number.MIN_VALUE;
    var plot_extent_min_x = Number.MAX_VALUE;
    var plot_extent_min_y = Number.MAX_VALUE;

    /* Get data points */
    for(var line_iter = 0; line_iter < lines.length; line_iter++) {
        if (lines[line_iter].localeCompare("") == 0) {
            continue;
        }
        var words = lines[line_iter].split(",");
        var cleaned_words = [];
        for(var word_iter = 0; word_iter < words.length; word_iter++) {
            var word = words[word_iter];
            var float_word = parseFloat(word);
            if(!isNaN(float_word)){
                cleaned_words.push(float_word);
            } 
        }

        if(cleaned_words.length != 2) {
            alert("Trying to plot data of dim > 2 or < 2 | length " + cleaned_words.length.toString());
            return;
        }

        plot_extent_max_x = Math.max(plot_extent_max_x, cleaned_words[0]);
        plot_extent_max_y = Math.max(plot_extent_max_y, cleaned_words[1]);
        plot_extent_min_x = Math.min(plot_extent_min_x, cleaned_words[0]);
        plot_extent_min_y = Math.min(plot_extent_min_y, cleaned_words[1]);

        x_pts.push(cleaned_words[0]);
        y_pts.push(cleaned_words[1]);
    }

    /* Get weights to draw weights-line */
    if(weights.localeCompare("") != 0) {
        weights_parts = weights.split(",");
        clean_weights = [];
        for(var wt_iter = 0; wt_iter < weights_parts.length; wt_iter++) {
            var float_wt = parseFloat(weights_parts[wt_iter]);
            if(!isNaN(float_wt)) {
                clean_weights.push(float_wt);
            } 
        }

        if(clean_weights.length != 2) {
            alert("Weights dont describe a line ! ");
            return;
        }

        /* Extend the plot extent; looks ugly without extending */
        plot_extent_max_x += 5;
        plot_extent_max_y += 5;
        plot_extent_min_x -= 5;
        plot_extent_min_y -= 5;

        /* Calculate 2 points at plot extents */ 
        line_x_pts.push(plot_extent_max_x);
        line_x_pts.push(plot_extent_min_x);
        line_y_pts.push(plot_extent_max_x * clean_weights[0] + clean_weights[1]);
        line_y_pts.push(plot_extent_min_x * clean_weights[0] + clean_weights[1]);
    }

    var scatter_data =  {
                            x: x_pts,
                            y: y_pts,
                            type: 'scatter',
                            mode: 'markers'
                        };
    if(weights.localeCompare("") != 0) {
        /* If we have valid weights; add to plot */
        line_data = {
                        x: line_x_pts,
                        y: line_y_pts,
                        mode: 'lines+markers'
                    };
    }

    var data = [scatter_data];
    data.push(line_data);

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

/*
 * Update the plot from data in input_path and weights_path.
 * If something goes wrong with the input points panic
 * and return; If something goes wrong with the weights
 * dont panic; Just dont update it
 */
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
                        if(str.localeCompare("-1") != 0) {
                            weights = str;
                        }
                        draw_plot(input_points, weights, cost);
                    }
                });
            }
        }
    });
}

/*
 * Print the weights in weights_path file
 * to the weights_output txt box.
 */
function display_weights(weights_path) {
    var parameter_map_weights = { 'fpath' : weights_path };
    $.ajax({
        url: "get_file_contents.php",
        type: "POST",
        data: parameter_map_weights,
        success: function(str) {
            if(str.localeCompare("-1") != 0) {
                document.getElementById('weights_output').value = str;
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

    var history_val = parseInt(h);
    $.ajax({
        url: "dump_file_status.php",
        type: "POST",
        data: parameter_map,
        success: function(str) {
            if(str.localeCompare("2") == 0) {
                /* Move to the results page */
                var history_val = parseInt(h);
                if(dims == 2) {
                    update_plot('./Results/' + uid + '_run/' + uid + '_input_' + (history_val + 1).toString() +  '.csv',
                                './Results/' + uid + '_run/' + uid + '_weights_' + (history_val + 1).toString() + '.csv');
                }
                /* Display weights */
                display_weights('./Results/' + uid + '_run/' + uid + '_weights_' + (history_val + 1).toString() + '.csv');

                /* Display refine form fields */
                document.getElementById('user_learning_rate').style.display = 'block';
                document.getElementById('user_epoch').style.display = 'block';
                document.getElementById('uploadFormSubmit').style.display = 'block';

                clearInterval(check_for_dumps_interval);
            } else if(str.localeCompare("0") == 0) {
                if(dims == 2) {
                    update_plot('./Results/' + uid + '_run/' + uid + '_input_dump.csv',
                                './Results/' + uid + '_run/' + uid + '_weights_dump.csv');
                }
                /* Display weights */
                display_weights('./Results/' + uid + '_run/' + uid + '_weights_dump.csv');
            }
        }
    });

} 

$(document).ready(
    function process_kickoff() {
        /* Hide refine form fields; Show when processing done */
        document.getElementById('user_learning_rate').style.display = 'none';
        document.getElementById('user_epoch').style.display = 'none';
        document.getElementById('uploadFormSubmit').style.display = 'none';

        if(dims == 2) {
            update_plot('./Results/' + uid + '_run/' + uid + '_input_' + h + '.csv',
                        './Results/' + uid + '_run/' + uid + '_weights_' + h + '.csv');
        } else {
            document.getElementById('plot_div').innerHTML = '<img src="./cant_draw.png" />';
        }
        display_weights('./Results/' + uid + '_run/' + uid + '_weights_' + h + '.csv');
        check_for_dumps_interval = setInterval(check_for_dumps, 250);
    }
);

$("#refineForm").submit(function(event) {
    event.preventDefault();

    var user_learning_rate = parseFloat(document.getElementById('user_learning_rate').value);
    var user_epoch = parseFloat(document.getElementById('user_epoch').value);

    if(isNaN(user_learning_rate) || user_learning_rate < 0) {
        /* Use default learning rate */
        user_learning_rate = 0;
    }

    if(isNaN(user_epoch) || user_epoch <= 0) {
        /* Using default value */
        user_epoch = -1;
    }

    var parameter_map = {
                        "uid"             : uid,
                        "learning_rate"   : user_learning_rate,
                        "epoch"           : user_epoch
                        };

    var history_val = parseInt(h);
    $.ajax({
        url: "process_csv.php",
        type: "POST",
        data: parameter_map,
        success: function(str) {
            if(str.localeCompare("0") == 0) {
                /* Move to the results page */
                window.location = 'result_refine.php?' + 'uid=' + uid + '&history=' + (history_val + 1).toString() + '&lr=' + user_learning_rate.toString() + '&epoch=' + user_epoch.toString();
            } else {
                alert(str + '... Aborting');
            }
        }
    });
});
