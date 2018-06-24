<?php  
function LoadJpeg($imgname)
{
    /* Attempt to open */
    $im = @imagecreatefromjpeg($imgname);

    /* See if it failed */
    if(!$im)
    {
        /* Create a black image */
        $im  = imagecreatetruecolor(150, 30);
        $bgc = imagecolorallocate($im, 255, 255, 255);
        $tc  = imagecolorallocate($im, 0, 0, 0);

        imagefilledrectangle($im, 0, 0, 150, 30, $bgc);

        /* Output an error message */
        imagestring($im, 1, 5, 5, 'Error loading ' . $imgname, $tc);
    }

    return $im;
}

header('Content-Type: image/jpeg');

$x=10;

while ($x<1000) {
	
$file = 'images/pic_'.$x.'.jpg';
$img = LoadJpeg($file);
imagejpeg($img);
imagedestroy($img);
sleep(1);
$x++;
if ($x >= 50) 
  $x = 0;
}

?>