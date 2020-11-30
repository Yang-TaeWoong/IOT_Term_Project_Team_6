
var devices_location = [{"latitude" : 33.450701,"longitude": 126.550667}, {"latitude" : 33.450701, "longitude":126.560662}, {"latitude":33.450701, "longitude":126.570647}];
var marker_positions = [];

var mapContainer = document.getElementById('map'), // 지도를 표시할 div  
    mapOption = { 
        center: new kakao.maps.LatLng(33.450701, 126.570667), // 지도의 중심좌표
        level: 3 // 지도의 확대 레벨
    };

var map = new kakao.maps.Map(mapContainer, mapOption); // 지도를 생성합니다
var count =0;
map.addOverlayMapTypeId(kakao.maps.MapTypeId.TERRAIN);

devices_location.forEach(function(element) {
	count+=1
	var markers_posi = new kakao.maps.LatLng(element["latitude"], element["longitude"])
	marker_positions.push({title:count,latlng:markers_posi})
});

marker_positions.forEach(function(position){
	// TODO : 상태를 읽어와서 적어야함.
	var marker = new kakao.maps.Marker({
		map : map,			
		position : position.latlng,
		title : position.title
	});	
})


// "http://know.nifos.go.kr/openapi/forestPoint/forestPointListSearch.do?localArea=&gubun=jeongug&keyValue=FcRDRyBL8ycJbMuJAjY3OI860dhBEJ3t6yKg2WoS1w0%3D&version=1.1&excludeForecast=1"


