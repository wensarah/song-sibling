let canvas, ctx;
let points = [];
let minX, maxX, minY, maxY;
//const colors = ['red', 'green', 'orange', 'purple', 'teal', 'magenta', 'brown', 'black', 'gold', 'navy', 'crimson'];

function drawAllPoints() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    points.forEach(p => {
        const px = ((p.x - minX) / (maxX - minX)) * canvas.width;
        const py = ((p.y - minY) / (maxY - minY)) * canvas.height;
        ctx.beginPath();
        ctx.arc(px, py, 5, 0, 2 * Math.PI);
        ctx.fillStyle = 'blue';
        ctx.fill();
    });
}

function highlightPoint(point, color) {
    const px = ((point.x - minX) / (maxX - minX)) * canvas.width;
    const py = ((point.y - minY) / (maxY - minY)) * canvas.height;
    ctx.beginPath();
    ctx.arc(px, py, 8, 0, 2 * Math.PI);
    ctx.fillStyle = color;
    ctx.fill();
}

fetch('http://localhost:8080/points')
    .then(res => res.json())
    .then(data => {
        points = data;

        canvas = document.getElementById('map');
        ctx = canvas.getContext('2d');

        const xs = points.map(p => p.x);
        const ys = points.map(p => p.y);
        minX = Math.min(...xs);
        maxX = Math.max(...xs);
        minY = Math.min(...ys);
        maxY = Math.max(...ys);

        drawAllPoints();

        canvas.addEventListener('click', (event) => {
            const px = event.offsetX;
            const py = event.offsetY;
            const qx = (px / canvas.width) * (maxX - minX) + minX;
            const qy = (py / canvas.height) * (maxY - minY) + minY;

            fetch(`http://localhost:8080/nearest?x=${qx}&y=${qy}`)
                .then(res => res.json())
                .then(nearest => {
                    drawAllPoints();
                    highlightPoint(nearest, 'red');
                    console.log('Nearest:', nearest.name);
                });
        });
 });

function drawClusters(clusters) {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    clusters.forEach((cluster, i) => {
        const color = colors[i % colors.length];
        cluster.forEach(p => {
            const px = ((p.x - minX) / (maxX - minX)) * canvas.width;
            const py = ((p.y - minY) / (maxY - minY)) * canvas.height;
            ctx.beginPath();
            ctx.arc(px, py, 6, 0, 2 * Math.PI);
            ctx.fillStyle = color;
            ctx.fill();
        });
    });
}
/*
document.getElementById('cluster').addEventListener('click', () => {
    fetch('http://localhost:8080/cluster?radius=15')
        .then(res => res.json())
        .then(clusters => drawClusters(clusters));
});*/