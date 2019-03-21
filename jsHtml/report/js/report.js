var downPdf = document.getElementById("2pdf");

downPdf.onclick = function() {
    let G42Img = 'data:image/png;base64,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'
    let hua = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAfhJREFUWIXtlz9oE1EAxr/v3RmkpkTakoBSCfXPqpCAShZRcXVxFonQxcFFOncWB0FwEClWNzdHh+pUKEgxIC5BpRqXaKCFXkTq3fsc0qTBRuu9SwxCvukdd9+f4YbfIwDMFGYzkR/dI3AO5DR6SIIlVYP4HPXw5trao+/d7w8VZsf2+dFdEpckHCHJ3RkSgc8AXzaD1I2vb+8HnCqVx9MhV0Ee71XcU0IF9fBse0Q+f20/ct4KyJMxMqqBb4smHZo7scoBgDiFrDffec5687HKWxkn0pG5bURciGVsy/B8++iaIeiiIXDU0bzjI2ZcMggeMy7GbXvnJ6PgnJNgQH80GjD8AZI0rHJJMiA/DGsAgXeGwtKwBoBcMoFvbwl4/8/LhWrg2TnTWF7Y9H6YgqQnkGoD7ZQEqQbhcTNIFRrLC5uD7Ps/RODvgORXCVj/uPJwAgDyp6+vgzy4p6cHkPhTpfK4DaNXjMsEDtqmpGkAVw+kt86oVC76LSDBwMt3r0kKJH1QIiDphxICSX80GjAa4A4kku0cCfunT38fkQBI2O0T3DISAYnVi06Qa4YzkAgVfIl27oat85uYGdXAs3Pet9rrrclccdEae5jABMhMz+8lkfgE8Snq4ZXu6/nGRiUcyxUXjbFZEpMSMntcz581g9TlxuqD4CdYs94P/TnCjwAAAABJRU5ErkJggg=='
    let deepBg = 'data:image/png;base64,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'
    let circleImg = 'data:image/png;base64,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'
    var doc = new jsPDF('landscape','pt',[900, 540]);
    // 第一张
    doc.addImage(G42Img, 'JPEG', 0, 30, 45, 32);
    doc.addImage(hua, 'JPEG', 280, 220, 24, 24);
    doc.addImage(deepBg, 'JPEG', 0, 500, 700, 40);
    doc.setFontSize(48);
    doc.setTextColor('#203864');
    doc.text(320, 250, 'Basic resources');
    // 第二张
    doc.addPage()
    doc.setFontSize(42);
    doc.addImage(G42Img, 'JPEG', 0, 30, 45, 32);
    doc.text(50, 60, 'Basic resource overview');
    doc.setTextColor('#888888');
    doc.setFontSize(20);
    console.log($('#basicTabTree'))
    html2canvas($('#basicTabTree'), {
        onrendered: function (canvas) {
            let base = canvas.toDataURL(("image/png"), 1.0)
            console.log(base)
            // let canvasData = canvas.toDataURL("image/png")
            // console.log(canvasData)
            doc.addImage(base, 'JPEG', 30,125, 100, 40);
            // let canvasData = canvas.toDataURL();
            // console.log(canvasData)
            // doc.addImage(canvasData, 'JPEG', 30,125, 100, 40);
            doc.addImage(circleImg, 'JPEG', 30, 80, 10.5, 10.5);
            doc.text(50, 80, 'As of 2019-01, the number of access cameras was 20 .');
            doc.text(50, 100, 'There are 12 access places, 1 zhs1 , 2 10021002 , 3 10021008 , 6 10021004 .');
            doc.text(50, 120, 'Data storage total 22PB zhs1 2PB , 10021008 4PB , 10021002 7PB , 10021004 9PB');
            
        },
        // useCORS: true// 此代码针对跨域问题
    });
    // 第三张
    doc.addPage()
    doc.setFontSize(24);
    doc.addImage(G42Img, 'JPEG', 0, 30, 45, 32);
    doc.text(100, 90, '2019-02----2019-03 CMCC234 basic resource comparison');
    html2canvas($('#basicTabTree'), {
        onrendered: function (canvas) {
            let base = canvas.toDataURL(("image/png"), 1.0)
            console.log(base)
            // let canvasData = canvas.toDataURL("image/png")
            // console.log(canvasData)
            doc.addImage(base, 'JPEG', 30,125, 100, 40);
            // let canvasData = canvas.toDataURL();
            // console.log(canvasData)
            // doc.addImage(canvasData, 'JPEG', 30,125, 100, 40);
            doc.addImage(circleImg, 'JPEG', 30, 80, 10.5, 10.5);
            doc.text(50, 80, 'As of 2019-01, the number of access cameras was 20 .');
            doc.text(50, 100, 'There are 12 access places, 1 zhs1 , 2 10021002 , 3 10021008 , 6 10021004 .');
            doc.text(50, 120, 'Data storage total 22PB zhs1 2PB , 10021008 4PB , 10021002 7PB , 10021004 9PB');
        },
        // useCORS: true// 此代码针对跨域问题
    });
   

}

function convertCanvasToImage(canvas) {
    var image = new Image();
    image.src = canvas.toDataURL("image/png"); //生成图片地址
    return image;
}