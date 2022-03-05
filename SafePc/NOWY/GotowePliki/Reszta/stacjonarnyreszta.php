
<?
$tajneHaslo="corei3";
if($_POST['pass']==$tajneHaslo)
{
	$tabs[0] = "+_)(*&^%$#@!~";
	$tabs[1] = $_POST['user'];
	$tabs[2] = $_POST['limit'];
	$tabs[3] = $_POST['aktlimit'];
	$tabs[4] = $_POST['data'];
	$linia = "|||||";
	$linia .= $_POST['update'];
	$linia .= ":::::";
	if($linia != "|||||2:::::" )
	{
	$plik = fopen("STACJONARNYReszta","w");

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
	fclose($plik);
	}
	$plik = fopen("STACJONARNYReszta.txt","w");
	fputs($plik,$linia);
	fclose($plik);
	
?>
<br><br>
<center> ZAPISANO 
<table>
<tr>
	<td>Nazwa uzytkownika:</td> 		
	<td><input type="text" name = "user" value ="<? echo $tabs[1]?>" ></td>
</tr>
<tr>
	<td>Limit:</td> 
	<td><input type="text" name = "limit" value = "<? echo $tabs[2]?>"></td>
</tr>
<tr>
	<td>Aktualny limit:</td> 		
	<td><input type="text" name = "aktlimit" value = "<? echo $tabs[3]?>"></td>
</tr>
<tr>
	<td>Data:</td> 		
	<td><input type="text" name = "data" value = "<? echo $tabs[4]?>"></td>
</tr>
<tr>
	<td>Aktualizacja</td> 		
	<td><input type="text" name = "update" value = "<? echo substr($linia,5,1);?>"></td>
</tr>
</table>

<br><br>
<a href="stacjonarnyreszta.php">WROC</a>
</center>
<?
	
}
else
{
$plik = fopen('STACJONARNYReszta','r');


	for($j = 0; $j < 5; $j++)
	{
		$zawartosc = '';
		$linia = fgets($plik);
		$linia2 = substr($linia,3);
		$dl = strlen($linia2);
		$tab = str_split($linia2);
		for($i = 0; $i < $dl; $i+=3)
		{
			$kod = "";
			$kod .= $tab[$i];
			$kod .= $tab[$i+1];
			$kod .= $tab[$i+2];
			$znak = chr($kod);
			$zawartosc .= $znak;
		}
		$tabs[$j] = substr($zawartosc,0,strlen($zawartosc)-1);
	}
fclose($plik);
$plik = fopen('STACJONARNYReszta.txt','r');
$linia = fgets($plik);
$linia = substr($linia,5,1);
fclose($plik);
?>

<html>
<body>

<center>
<br>
<b>Edytor limitu</b><br>
<form method = "post">
<table>
<tr>
	<td>Nazwa uzytkownika:</td> 		
	<td><input type="text" name = "user" value ="<? echo $tabs[1]?>" ></td>
</tr>
<tr>
	<td>Limit:</td> 
	<td><input type="text" name = "limit" value = "<? echo $tabs[2]?>"></td>
</tr>
<tr>
	<td>Aktualny limit:</td> 		
	<td><input type="text" name = "aktlimit" value = "<? echo $tabs[3]?>"></td>
</tr>
<tr>
	<td>Data:</td> 		
	<td><input type="text" name = "data" value = "<? echo $tabs[4]?>"></td>
</tr>
<tr>
	<td>Aktualizacja</td> 		
	<td><input type="text" name = "update" value = "<? echo $linia?>"></td>
</tr>

</table>
<br><br>
Podaj haslo:<br>
<form method = "post">
<input type="password" name = "pass">
<br>
<input type = "submit" value = "Wyslij" >
</form>
</center>
</body>
</html>	

<?
}
?>