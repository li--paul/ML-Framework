var scatter_data =  {
                        x: [-1, 2, 3, 4, 5, -6, 7, -8, 9, -10],
                        y: [12, -22, 28, -39, 55, 58, -68, 82, -87, 95],
                        type: 'scatter',
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
