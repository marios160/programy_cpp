<?
	$tabs[0] = "+_)(*&^%$#@!~";
	$tabs[1] = "@Mateusz";
	$tabs[2] = "*60";
	$tabs[3] = "*0";
	$tabs[4] = "*321312412";

$plik = fopen("MATIMateuszs","w");

for($j = 0; $j < 5; $j++)
	{
		$tab = str_split($tabs[$j]);
		$dl = strlen($tabs[$j]);
		$zawartosc = "100";
		for($i = 0; $i < $dl; $i++)
		{
			$znak = ord($tab[$i])/100;
			while(strlen($znak)<4)
			{
				$znak.= "0";
			}
			if(strpos($znak,"."))
			{
				$znak = substr($znak,0,1).substr($znak,strpos($znak,".")+1);
			}
			$zawartosc .= $znak;
			
		}
		fputs($plik,$zawartosc."\n");
	}
	
	?>