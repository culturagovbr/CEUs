<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:importasisma.inc
   Finalidade: permitir importar os dados do SISMA
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
if ($_REQUEST['recebe']=='recebe')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo.inc";

}
if ($_REQUEST['recebe']=='recebe2')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase2.inc";

}
if ($_REQUEST['recebe']=='recebe3')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase3.inc";

  }
if ($_REQUEST['recebe']=='recebe4')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/importatudo_fase4.inc";
}  
if ($_REQUEST['recebe']=='migra')
{
  unset ($arquivo);
  include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/comunica/migrasisma.inc";
}
?>
<br>
<div class="notprint">
<?
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Importa��o de Dados do SISMA';
monta_titulo($titulo_modulo,'');
$anoorigem=$_REQUEST['anoorigem'];
$anodestino=$_REQUEST['anodestino'];

?>
<html>
<head>
<title>Importa��o da Dados do SISMA</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="recebe" >
    <center>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	 <tr>
	 <td ><!--<input type='button' class="botao" value='Receber todos (exceto a��es, financeiro e f�sico) ' onclick="recebe_dados('1')">
<input type='button' class="botao" value='Receber A��es' onclick="recebe_dados('4')">	 
	 <input type='button' class="botao" value='Receber dados Financeiros' onclick="recebe_dados('2')">
	 <input type='button' class="botao" value='Receber dados F�sicos' onclick="recebe_dados('3')">
   --> </td>
	 </tr>
	 <tr>
	 <td ><HR> </td>
	 </tr>
	 <tr>
	 <td ><B>MIGRA��O DE DADOS </b></td>
	 </tr>
</table>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='left' class="SubTituloDireita">Exerc�cio base de compara��o:</td>
        <td><?=campo_texto('anoorigem','N','S','',6,4,'####','');?>  </td>
      </tr>	
      <tr>
        <td align='left' class="SubTituloDireita">O que ser� migrado:</td>
        <td><input type='checkbox' name='migrap' value='r' <?if ($_REQUEST['resp']) print '   checked'?>>Usu�rios com respectivos Perfis<br>

        
        </td>
      </tr>	 
      <tr>
        <td align='center' colspan="2" >
        <input type='button' class="botao" value='Executar Migra��o' onclick="migra_dados()"></td>
      </tr>	


  </table>
</form> 
<script>
function recebe_dados(cod)
{
  if( window.confirm( "Este processo pode levar v�rios minutos. \n Confirma a importa��o de dados do SIGPLAN ?") )
    {
      if (cod=='1'){
  document.formulario.recebe.value='recebe';
    document.formulario.submit();}
  if (cod=='2') {
     document.formulario.recebe.value='recebe2';
  document.formulario.submit();}

  if (cod=='3') {
     document.formulario.recebe.value='recebe3';
  document.formulario.submit();}
}
  if (cod=='4') {
     document.formulario.recebe.value='recebe4';
  document.formulario.submit();}

}


function migra_dados()
{
	if (!validaBranco(document.formulario.anoorigem, 'Exerc�cio base de compara��o')) return ;

	if (! document.formulario.migrap.checked ) {alert ('Escolher a a��o de migra��o!'); return;}
		document.formulario.recebe.value='migra';
        document.formulario.submit();	
}


</script>
</body>
</html>
