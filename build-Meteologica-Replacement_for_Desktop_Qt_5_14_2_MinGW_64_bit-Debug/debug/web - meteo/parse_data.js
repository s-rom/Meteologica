$(document).ready(function(){
    fr.onload = function() {
        parse_data(fr.result);
        $('#raw_json_data').text(fr.result);
    };
})

function parse_data(json_data) {
    $('#prevision_table_body').empty();

    try{
        meteo_data = JSON.parse(json_data);
        let city = meteo_data.City;
        let units = meteo_data.TemperatureUnits;

        $('#city_header').text(city);

        for (let i = 0; i < meteo_data.Records.length; i++){
            let date = meteo_data.Records[i].Date;
            let minTemp = meteo_data.Records[i].MinTemp;
            let maxTemp = meteo_data.Records[i].MaxTemp;
            let precipitation = meteo_data.Records[i].Precipitation;
            let cloudiness = meteo_data.Records[i].Cloudiness;

            $('#prevision_table> tbody:last-child').
            append('<tr class=\'table\'><td>'
                    +date+'</td><td>'
                    +"["+minTemp+", "+maxTemp+'] º'+
                    (units=="Celsius"?"C":"F")+'</td><td>'
                    +precipitation+' mm</td><td>'
                    +cloudiness+' %</td><td>'
                    +'</tr>');                   
        }
    }
    catch(e){
        //console.log("Output is not valid json");
    }
}