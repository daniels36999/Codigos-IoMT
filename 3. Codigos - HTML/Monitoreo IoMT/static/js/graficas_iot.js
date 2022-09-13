
//<!-- GRAFICA TEMPERATURA -->
//---------------------------------------------------------
var Temp = "";       var Fec = "";
var Temperatura =[]; var Fecha =[];

$(document).ready(function() {
  $("#btnGraficar").click(function() {
	$(".GF").parent("tr").find(".GF").each(function() {
	 Fec +=$(this).html()+",";
	});
	$(".GT").parent("tr").find(".GT").each(function() {
	 Temp +=$(this).html()+",";
	});
	
	Temp=Temp.split(',')
	TempLen = Temp.length-1;
	Fec=Fec.split(',')
	for ( var i = 0; i < TempLen; i++ ) {
		Temperatura[i]=Temp[i];
		Fecha[i]=Fec[i];
		}
	var ctx= document.getElementById("GraTemperatura").getContext("2d");
	let gradient=ctx.createLinearGradient(0,0,0,400);
	gradient.addColorStop(0,"rgba(144,0,0,0.7)");
	gradient.addColorStop(1,"rgba(200,0,0,0.3)");
	var myChart= new Chart(ctx,{
		type:"line", //<!--TIPO DE GRAFICA -->
		data:{
			labels:Fecha, //<!--DATO FECHA -->		
			datasets:[{
					label:'TEMPERATURA CORPORAL',
					data:Temperatura, //<!--DATO TEMPERATURA -->
					backgroundColor: gradient,
					borderColor: "#000000",
					borderWidth:4,
						}]},
		options:{
			responsive: false,
			title: {
			 display: true,
			 text: 'HISTORIAL DE LA TEMPERATURA CORPORAL'
			  },
			
			scales:{
				yAxes:[{
					scaleLabel: {
						display: true,
						labelString: "TEMPERATURA [°C]",},
					ticks:{
						beginAtZero:true							
						}}],				
				xAxes:[{
					scaleLabel: {
						display: true,
						labelString: "FECHA [dd/mm/aa]",
					}}]												
		}}});	
		
	document.getElementById('oli').style.display = 'block';
	$("#DescTemp").click(function() {
	   myChart.options.title.text = 'Temperatura Corporal - Historial';
	   myChart.update({
		  duration: 0
	   });
	   var link = document.createElement('a');
	   link.href = myChart.toBase64Image();
	   link.download = 'Temperatura Corporal.png';
	   link.click();
	   myChart.options.title.text = 'HISTORIAL: TEMPERATURA CORPORAL';
	   myChart.update({
		  duration: 0
	   });
	   console.log("holaaaa")
	});
		
  });  
  
});



//<!-- GRAFICA PESO -->
//---------------------------------------------------------
var Pes = ""; var Fec = "";
var Peso =[]; var Fecha =[];

$(document).ready(function() {
  $("#btnGraficar").click(function() {
	$(".GF").parent("tr").find(".GF").each(function() {
	 Fec +=$(this).html()+",";
	});
	$(".GP").parent("tr").find(".GP").each(function() {
	 Pes +=$(this).html()+",";
	});
	
	Pes=Pes.split(',')
	PesLen = Pes.length-1;
	Fec=Fec.split(',')
	for ( var i = 0; i < PesLen; i++ ) {
		Peso[i]=Pes[i];
		Fecha[i]=Fec[i];
		}

	var ctx= document.getElementById("GraPeso").getContext("2d");
	let gradient=ctx.createLinearGradient(0,0,0,400);
	gradient.addColorStop(0,"rgba(0,144,0,0.7)");
	gradient.addColorStop(1,"rgba(0,200,0,0.1)");
	var myChart= new Chart(ctx,{
		type:"line", //<!--TIPO DE GRAFICA -->
		data:{
			labels:Fecha, //<!--DATO FECHA -->		
			datasets:[{
					label:'PESO CORPORAL',
					data:Peso, //<!--DATO Peso -->
					backgroundColor: gradient,
					borderColor: "#000000",
					pointBackgroundColor: "#124500",
					pointBorderColor: "#124500",	
					borderWidth:4,
						}]},
		options:{
			responsive: false,
			title: {
			 display: true,
			 text: 'HISTORIAL: PESO CORPORAL'
			  },
			scales:{
				yAxes:[{
					scaleLabel: {
						display: true,
						labelString: "PESO [Kg]",},
					ticks:{
						beginAtZero:true							
						}}],				
				xAxes:[{
					scaleLabel: {
						display: true,
						labelString: "FECHA [dd/mm/aa]",
					}}]												
		}}});	
	document.getElementById('Separa_Grafica').style.display = 'block';
	document.getElementById('btnAnimacion').style.display = 'block';	
	$("#DescPes").click(function() {
	   myChart.options.title.text = 'Peso Corporal - Historial';
	   myChart.update({
		  duration: 0
	   });
	   var link = document.createElement('a');
	   link.href = myChart.toBase64Image();
	   link.download = 'Peso Corporal.png';
	   link.click();
	   myChart.options.title.text = 'HISTORIAL: PESO CORPORAL';
	   myChart.update({
		  duration: 0
	   });
	   console.log("holaaaa")
	});	
		
  });
});

//<!-- GRAFICA ESTATURA -->
//---------------------------------------------------------
var Est = ""; var Fec = "";
var Estatura =[]; var Fecha =[];

$(document).ready(function() {
  $("#btnGraficar").click(function() {
	$(".GF").parent("tr").find(".GF").each(function() {
	 Fec +=$(this).html()+",";
	});
	$(".GE").parent("tr").find(".GE").each(function() {
	 Est +=$(this).html()+",";
	});
	
	Est=Est.split(',')
	EstLen = Est.length-1;
	Fec=Fec.split(',')
	for ( var i = 0; i < EstLen; i++ ) {
		Estatura[i]=Est[i];
		Fecha[i]=Fec[i];
		}

	var ctx= document.getElementById("GraEstatura").getContext("2d");
	let gradient=ctx.createLinearGradient(0,0,0,400);
	gradient.addColorStop(0,"rgba(0,0,144,0.7)");
	gradient.addColorStop(1,"rgba(0,0,200,0.1)");
	var myChart= new Chart(ctx,{
		type:"line", //<!--TIPO DE GRAFICA -->
		data:{
			labels:Fecha, //<!--DATO FECHA -->		
			datasets:[{
					label:'ESTATURA CORPORAL',
					data:Estatura, //<!--DATO Estatura -->
					backgroundColor: gradient,
					borderColor: "#000000",
					pointBackgroundColor: "#124500",
					pointBorderColor: "#124500",
					borderWidth:4,					
						}]},
		options:{
			responsive: false,
			title: {
			 display: true,
			 text: 'HISTORIAL: ESTATURA CORPORAL'
			  },
			scales:{
				yAxes:[{
					scaleLabel: {
						display: true,
						labelString: "ESTATURA [m]",},
					ticks:{
						beginAtZero:true							
						}}],				
				xAxes:[{
					scaleLabel: {
						display: true,
						labelString: "FECHA [dd/mm/aa]",
					}}]												
		}}});	
		
	$("#DescEst").click(function() {
	   myChart.options.title.text = 'Estatura Corporal - Historial';
	   myChart.update({
		  duration: 0
	   });
	   var link = document.createElement('a');
	   link.href = myChart.toBase64Image();
	   link.download = 'Estatura Corporal.png';
	   link.click();
	   myChart.options.title.text = 'HISTORIAL: ESTATURA CORPORAL';
	   myChart.update({
		  duration: 0
	   });
	   console.log("holaaaa")
	});
  });
});

//<!-- GRAFICA IMC -->
//---------------------------------------------------------

var Im = ""; var Fec = "";
var IMC =[]; var Fecha =[];

$(document).ready(function() {
  $("#btnGraficar").click(function() {
	$(".GF").parent("tr").find(".GF").each(function() {
	 Fec +=$(this).html()+",";
	});
	$(".GI").parent("tr").find(".GI").each(function() {
	 Im +=$(this).html()+",";
	});
	
	Im=Im.split(',')
	ImcLen = Im.length-1;
	Fec=Fec.split(',')
	for ( var i = 0; i < ImcLen; i++ ) {
		IMC[i]=Im[i];
		Fecha[i]=Fec[i];
		}

	var ctx= document.getElementById("GraImc").getContext("2d");
	let gradient=ctx.createLinearGradient(0,0,0,400);
	gradient.addColorStop(0,"rgba(200,0,255,0.8)");
	gradient.addColorStop(1,"rgba(126, 2, 252, 0.3)");
	var myChart= new Chart(ctx,{
		type:"line", //<!--TIPO DE GRAFICA -->
		data:{
			labels:Fecha, //<!--DATO FECHA -->		
			datasets:[{
					label:'INDICE DE MASA CORPORAL',
					data:IMC, //<!--DATO IMC -->
					backgroundColor:gradient,
					borderColor: "#000000",
					pointBackgroundColor: "#124500",
					pointBorderColor: "#124500",
					borderWidth:4,
						}]},
		options:{
			responsive: false,
			title: {
			 display: true,
			 text: 'HISTORIAL: INDICE DE MASA CORPORAL'
			  },
			scales:{
				yAxes:[{
					scaleLabel: {
						display: true,
						labelString: "IMC [Kg/m^2]",},
					ticks:{
						beginAtZero:true							
						}}],				
				xAxes:[{
					scaleLabel: {
						display: true,
						labelString: "FECHA [dd/mm/aa]",
					}}]												
		}}});	
	$("#DescImc").click(function() {
	   myChart.options.title.text = 'IMC - Historial';
	   myChart.update({
		  duration: 0
	   });
	   var link = document.createElement('a');
	   link.href = myChart.toBase64Image();
	   link.download = 'IMC.png';
	   link.click();
	   myChart.options.title.text = 'HISTORIAL: INDICE DE MASA CORPORAL';
	   myChart.update({
		  duration: 0
	   });
	   console.log("holaaaa")
	});	
  });
});




//<!-- GRAFICA O2Sat -->
//---------------------------------------------------------
var O2S = "";  var Fec = "";
var O2Sat =[]; var Fecha =[];

$(document).ready(function() {
  $("#btnGraficar").click(function() {
	$(".GF").parent("tr").find(".GF").each(function() {
	 Fec +=$(this).html()+",";
	});
	$(".GO").parent("tr").find(".GO").each(function() {
	 O2S +=$(this).html()+",";
	});
	
	O2S=O2S.split(',')
	O2SLen = O2S.length-1;
	Fec=Fec.split(',')
	for ( var i = 0; i < O2SLen; i++ ) {
		O2Sat[i]=O2S[i];
		Fecha[i]=Fec[i];
		}

	var ctx= document.getElementById("GraO2Sat").getContext("2d");
	let gradient=ctx.createLinearGradient(0,0,0,400);
	gradient.addColorStop(0,"rgba(255, 248, 8, 0.9)");
	gradient.addColorStop(1,"rgba(255, 248, 8, 0.2)");
	var myChart= new Chart(ctx,{
		type:"line", //<!--TIPO DE GRAFICA -->
		data:{
			labels:Fecha, //<!--DATO FECHA -->		
			datasets:[{
					label:'O2SAT',
					data:O2Sat, //<!--DATO O2Sat -->
					backgroundColor: gradient,
					borderColor: "#000000",
					pointBackgroundColor: "#124500",
					pointBorderColor: "#124500",	
						}]},
		options:{
			responsive: false,
			title: {
			 display: true,
			 text: 'HISTORIAL: SATURACION DE OXIGENO EN LA SANGRE'
			  },
			scales:{
				yAxes:[{
					scaleLabel: {
						display: true,
						labelString: "O2SAT [%]",},
					ticks:{
						beginAtZero:true							
						}}],				
				xAxes:[{
					scaleLabel: {
						display: true,
						labelString: "FECHA [dd/mm/aa]",
					}}]												
		}}});	
	$("#DescO2Sat").click(function() {
	   myChart.options.title.text = 'O2Sat - Historial';
	   myChart.update({
		  duration: 0
	   });
	   var link = document.createElement('a');
	   link.href = myChart.toBase64Image();
	   link.download = '=2SAT.png';
	   link.click();
	   myChart.options.title.text = 'HISTORIAL: SATURACION DE OXIGENO EN LA SANGRE';
	   myChart.update({
		  duration: 0
	   });
	   console.log("holaaaa")
	});		
		
  });
});

//EXPORTAR TODOS LOS GRAFICOS
//-------------------------------------------------
$('#DescPDF').click(function(event) { 
	var reportPageHeight = 1500;
	var reportPageWidth = 2200;
	var pdfCanvas = $('<canvas />').attr({
    id: "canvaspdf",
    width: reportPageWidth,
    height: reportPageHeight
  });
  
  var pdfctx = $(pdfCanvas)[0].getContext('2d');
  var pdfctxX = 0;
  var pdfctxY = 0;
  var buffer = 100;
  
  $("canvas").each(function(index) {
    var canvasHeight = $(this).innerHeight();
    var canvasWidth = $(this).innerWidth();
    
    pdfctx.drawImage($(this)[0], pdfctxX, pdfctxY, canvasWidth, canvasHeight);
    pdfctxX += canvasWidth + buffer;
    
    if (index % 2 === 1) {
      pdfctxX = 0;
      pdfctxY += canvasHeight + buffer;
    }
  });
  
  var pdf = new jsPDF('l', 'pt', [reportPageWidth, reportPageHeight]);
  pdf.addImage($(pdfCanvas)[0], 'PNG', 0, 0);
  pdf.save('filename.pdf');
});

function done(){
  alert("haha");
  var url=myLine.toBase64Image();
  document.getElementById("url").src=url;
}

