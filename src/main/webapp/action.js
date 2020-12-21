var marker_positions = [
	{
		content: '<div>카카오</div>',
		latlng: new kakao.maps.LatLng(33.450701, 126.570677)
	},
	{
		content: '<div>생태연못</div>',
		latlng: new kakao.maps.LatLng(33.450936, 126.569477)
	},
	{
		content: '<div>텃밭</div>',
		latlng: new kakao.maps.LatLng(33.450879, 126.569940)
	}
];
function info_window_content(device_name) {
	var end_point_info = '<div class="overlaybox">' +
		'    <div class="boxtitle">data</div>' +
		'    <div class="first">' +
		'        <div class="triangle text">degree</div>' +
		'        <div class="movietitle text">' + '드래곤 길들이기2' + '</div>' +
		'    </div>' +
		'    <ul>' +
		'        <li class="up">' +
		'            <span class="number">2</span>' +
		'            <span class="title">' + device_name + '</span>' +
		'            <span class="arrow up"></span>' +
		'            <span class="count">2</span>' +
		'        </li>' +
		'        <li>' +
		'            <span class="number">3</span>' +
		'            <span class="title">' + device_name + '</span>' +
		'            <span class="arrow up"></span>' +
		'            <span class="count">6</span>' +
		'        </li>' +
		'        <li>' +
		'            <span class="number">3</span>' +
		'            <span class="title">' + device_name + '</span>' +
		'            <span class="arrow up"></span>' +
		'            <span class="count">6</span>' +
		'        </li>' +
		'    </ul>' +
		'</div>';
	return end_point_info;
}

function call_ajax(url)
{
	 var xmlhttp;
    // compatible with IE7+, Firefox, Chrome, Opera, Safari
    xmlhttp = new XMLHttpRequest();
    xmlhttp.onreadystatechange = function(){
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200){
            callback(xmlhttp.responseText);
        }
    }
    xmlhttp.open("POST", url, true);
    xmlhttp.send();
}

function load_map() {
	var mapContainer = document.getElementById('contents'), // 지도를 표시할 div  
		mapOption = {
			center: new kakao.maps.LatLng(33.450701, 126.570667), // 지도의 중심좌표
			level: 3 // 지도의 확대 레벨
		};

	var map = new kakao.maps.Map(mapContainer, mapOption); // 지도를 생성합니다
	map.addOverlayMapTypeId(kakao.maps.MapTypeId.TERRAIN);

	marker_positions.forEach(function(position) {
		// TODO : 상태를 읽어와서 적어야함.
		var marker = new kakao.maps.Marker({
			map: map,
			position: position.latlng
		});
		var infowindow = new kakao.maps.InfoWindow({
			content: info_window_content("data")
		});
		kakao.maps.event.addListener(marker, 'mouseover', makeOverListener(map, marker, infowindow));
		kakao.maps.event.addListener(marker, 'mouseout', makeOutListener(infowindow));
	})
}
function makeOverListener(map, marker, infowindow) {
	call_ajax('/status');
	return function() {
		infowindow.open(map, marker);
	}
}

function makeOutListener(infowindow) {
	return function() {
		infowindow.close();
	};
}

window.addEventListener("load", load_map);
var map_start_btn = document.querySelector("#map_btn");
map_start_btn.addEventListener("click", load_map);
var camera_start_btn = document.querySelector("#camera_btn");
camera_start_btn.addEventListener("click", function() {
	document.querySelector("#contents").innerHTML = "hi";
	//TODO : send a query to specific url
});

// "http://know.nifos.go.kr/openapi/forestPoint/forestPointListSearch.do?localArea=&gubun=jeongug&keyValue=FcRDRyBL8ycJbMuJAjY3OI860dhBEJ3t6yKg2WoS1w0%3D&version=1.1&excludeForecast=1"


