<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:propostarevisao.inc
   Finalidade: eramitir a constru��o de propostas de revis�o
   */
$modulo=$_REQUEST['modulo'] ;//

$titulo_modulo='Proposta de Fus�o de A��o';
monta_titulo($titulo_modulo,'');

$sql= "select a.* from elabrev.ppaacao_proposta a where a.acastatus='N' and a.acaid = ".$_REQUEST['acaid'];


if (! $_REQUEST['acaid'] or $_REQUEST['acaid']=='') {
	?>
	<script>alert('Ocorreu um problema ao abrir a p�gina. Tente novamente!');
	history.back();</script>
	<?
	$db->close();
	exit();
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
if ($nlinhas < 0 ){
	?>
	<script>alert('A��o Inexistente!');
	history.back();</script>
	<?
	$db->close();
	exit();}
else {
  $res =  @$db->carrega_registro($RS,0);
}
// Transforma em vari�veis todos os campos do array
if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

if ($_REQUEST['decisao'])
{
	// decidiu
	$sql = "update elabrev.proposta_fusao_acao set pfadata=now() ,pfastatus='D' where pfaid=".$_REQUEST['decisao'];  	 
  	$db->executar($sql);
	$db->commit();
	?>
      <script>
         alert('Grava��o com sucesso!')
        // window.close();
      </script>
    <?
 //exit();
}

if ($_REQUEST['cdecisao'])
{
	// cancelou a decis�o
	$sql = "update elabrev.proposta_fusao_acao set pfadata=now(),pfastatus='A' where pfaid=".$_REQUEST['cdecisao'];  	 
  	$db->executar($sql);
	 $db->commit();
	   ?>
      <script>
         alert('Grava��o com sucesso!')
         //window.close();
      </script>
  <?
 // exit();
}

if ($_REQUEST['grava'] and $_REQUEST['pfajustificativa'])    
{
  if ($_REQUEST['inserir']==1)
  {  	  	  	
  	$acaid_externa = $_REQUEST['acaid_combo'];
  	$sql = "insert into elabrev.proposta_fusao_acao (acaid,usucpf,acaid_externa,pfajustificativa,prsano)
       values (".
  	$_REQUEST['acaid'].",'".
  	$_SESSION['usucpf']."',".
  	$acaid_externa.",'".
  	$_REQUEST['pfajustificativa']."','".$_SESSION['exercicio']."')";
  	$db->executar($sql); 

  	// uma vez que ocorre a inclus�o, pego o valor do pfaid  	 	
  	$sql = "select pfaid from elabrev.proposta_fusao_acao where acaid=".$_REQUEST['acaid']." and usucpf='".$_SESSION['usucpf']."' and acaid_externa=$acaid_externa ";
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;

    // agora acho os dados identificadores da outra a��o
    $sql = "select acacod as acacode, acadsc as acadsce from elabrev.ppaacao_proposta where acaid=".$acaid_externa;

	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;	
	
	
  	// inserir uma proposta de exclus�o da a��o que fundiu
  	
	$sql= "insert into elabrev.proposta_exclusao_acao (usucpf,peajustificativa,acaid,peaorigem,prsano) values ('".
  	$_SESSION['usucpf']."',".
  	"'Fus�o com a a��o $acacod',$acaid_externa,'F','".$_SESSION['exercicio']."')";
    $db->executar($sql); 	
	$db->commit();
	$pfaid=0;
	?>
      <script>
         alert('Grava��o com sucesso.')
      </script>
  <?
  //exit();
  }
  if ($_REQUEST['inserir']==2)
  {
     $sql = "update elabrev.proposta_fusao_acao set pfadata=now(), pfajustificativa='".$_REQUEST['pfajustificativa']."' where pfaid=".$_REQUEST['pfaid'];
  	 $db->executar($sql);
	 $db->commit();
	   ?>
      <script>
         alert('Grava��o com sucesso.')
      </script>
  <?
    }
}


if ($_REQUEST['edita'] and $_REQUEST['inserir']==3)    
{
	$sql = "select * from proposta_fusao_acao where pfaid=".$_REQUEST['edita'];
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  	
    $sql="select distinct pfaid from elabrev.proposta_fusao_acao p where p.pfastatus in ('A','D') and p.acaid=".$_SESSION['acaid']." and usucpf='".$_SESSION['usucpf']."'"; 	 
	$RS = $db->record_set($sql);
	$nlinhas=$db->conta_linhas($RS);
	
	$sql = "delete from elabrev.proposta_fusao_acao where pfaid=".$_REQUEST['edita'];
  	$db->executar($sql);

	$sql= "delete from elabrev.proposta_exclusao_acao where usucpf='".$_SESSION['usucpf']."' and acaid=$acaid_externa and peaorigem='F'";
  	$db->executar($sql);
	   $db->commit();
	     	   $pfaid=0;
  	   $pfajustificativa='';
	   ?>
      <script>
         alert('Exclus�o realizada com sucesso!')
      </script>
  <?

}
$habil='S';
if ($_REQUEST['edita'] and $_REQUEST['inserir']==2)    
{
	$sql = "select * from proposta_fusao_acao where pfaid=".$_REQUEST['edita'];
	$RS = $db->record_set($sql);
	$res =  @$db->carrega_registro($RS,0);
	if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	$acaid_combo=$acaid_externa;
	$habil='N';
}
?>

<html>
<head>
<title>Proposta de Fus�o</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<script language="JavaScript" src="../includes/funcoes.js"></script>
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
<input type=hidden name="modulo" value="<?=$modulo?>">
<input type=hidden name="decisao" value="0">
<input type=hidden name="cdecisao" value="0">
<input type=hidden name="grava" value="0">
<input type=hidden name="edita" value="0">
<? if ($pfaid) {?>
<input type=hidden name="inserir" value="2">
<input type=hidden name="pfaid" value="<?=$pfaid?>">
<?} else {?>
<input type=hidden name="inserir" value="1">
<?}?>
<center>
    <table width='95%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
      <tr>
        <td align='right' class="subtitulodireita">A��o:</td>
        <td><?=$prgcod.'.'.$acacod.'-'.$acadsc?></td>
      </tr>  
 	  <tr>
        <td align="right" class="subtitulodireita">Escolha a a��o:</td>
        <? 
        if ($habil <> 'N')
           $sql = "select distinct a.acaid as codigo, a.prgcod||'-'||a.acacod||'-'||a.acadsc as descricao from elabrev.ppaacao_proposta a where a.acaid !=".$_SESSION['acaid']." and a.acastatus='A' and a.acaid not in (select pf.acaid_externa from elabrev.proposta_fusao_acao pf where pf.usucpf='".$_SESSION['usucpf']."' and pf.prsano='".$_SESSION['exercicio']."') order by descricao";
        else
        { 
           $sql = "select distinct a.acaid as codigo, a.prgcod||'-'||a.acacod||'-'||a.acadsc as descricao from elabrev.ppaacao_proposta a where a.acaid !=".$_SESSION['acaid']." and a.acastatus='A'  order by descricao";
           
        }

        ?><td colspan=2><?=$db->monta_combo("acaid_combo",$sql,$habil,"Escolha a a��o que ser� fundida.",'','',$explicacao,400);?>   
      
        </td>
     </tr>
	  <tr>
        <td align="right" class="subtitulodireita">Justificativa:</td> 
       <td colspan=2><?=campo_textarea('pfajustificativa','N','S','','80',5,'');?></td>
     </tr>
	 <tr>
	 <td colspan="2" align="right" class="subtitulodireita">
	 <? if (! $pfaid) {?>
	 <input type='button' class="botao" value='Inserir!' onclick="grava_proposta()">
	 <?} else {?>
	 <input type='button' class="botao" value='Alterar!' onclick="grava_proposta()">	 
	 <?}?>
	 &nbsp;&nbsp;&nbsp;<input type='button' class="botao" value='Fechar' onclick="fechar_janela()"></td>
	 </tr>
  </table>
</form> 

<?
// lista as propostas j� feitas
$cabecalho = array('Status','Autor','Proposta de Fus�o');
if ($db->verifica_momento() and $db->verifica_resp_elabrev())
	$sql="select distinct case when p.pfastatus='A' then '<img border=\"0\" src=\"../imagens/principal.gif\" title=\" Editar proposta \" onclick=\"edita_proposta(' || p.pfaid|| ')\">
	<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir proposta \" onclick=\"exclui_proposta(' || p.pfaid|| ')\">	
	<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pfaid|| ')\">' else 
	'<img border=\"0\" src=\"../imagens/IconeAjuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pfaid|| ')\">' end as acao ,'<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || u.usucpf || '\'' || ')\">&nbsp;'||u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as autor, a.acadsc||'<br> Digitado em '||to_char(p.pfadata,'YYYY/mm/dd') as dado 
from elabrev.proposta_fusao_acao p inner join seguranca.usuario u on p.usucpf=u.usucpf inner join elabrev.ppaacao_proposta a on a.acaid=p.acaid_externa where p.pfastatus in ('A','D') and p.acaid=".$_SESSION['acaid']." order by dado desc, acao, autor";
else 
	$sql="select distinct  '<img border=\"0\" src=\"../imagens/email.gif\" title=\" Enviar E-mail \" onclick=\"envia_email(' || '\'' || u.usucpf || '\'' || ')\"><img border=\"0\" src=\"../imagens/ajuda.gif\" title=\" Ver justificativa \" onclick=\"ver_justificativa(' || p.pfaid|| ')\">' as acao,case when p.pfastatus='A' then 'Prop.' when p.pfastatus='D' then 'Dec.' end as status,u.usunome||' - '||u.usufoneddd||'-'||u.usufonenum as autor,to_char(p.pfadata,'YYYY/mm/dd') as data, '<br> Digitado em '||to_char(p.pfadata,'YYYY/mm/dd') as dado from elabrev.proposta_fusao_acao p inner join seguranca.usuario u on p.usucpf=u.usucpf inner join elabrev.ppaacao_proposta a on a.acaid=p.acaid_externa where p.pfastatus in ('A','D') and  p.acaid=".$_SESSION['acaid']."  and p.prsano='".$_SESSION['exercicio']."' order by dado desc, acao, autor";

    $db->monta_lista($sql,$cabecalho,100,20,'','','');

?>

<script>
  function fechar_janela()
  {
    window.close();

  }
    function grava_proposta()
  {
  	try
  	{
  	    if (!validaBranco(document.formulario.acaid_combo, 'A��o')) return;
  	}
  	catch(e)
		{
			
		}
  	if (!validaBranco(document.formulario.pfajustificativa, 'Justificativa')) return;  	
	document.formulario.grava.value = 1;
	document.formulario.submit();
  }
    function edita_proposta(cod)
  {
  	document.formulario.inserir.value = 2;  
	document.formulario.edita.value = cod;
	document.formulario.submit();
  }  
    function exclui_proposta(cod)
  {
	document.formulario.inserir.value = 3;  	
	document.formulario.edita.value = cod;
	document.formulario.submit();
  }  
   function envia_email(cpf)
  {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/envia_email&acao=A&cpf="+cpf;
          window.open(e, "Envioemail","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
  }

  function ver_justificativa(cod)
  {
          e = "<?=$_SESSION['sisdiretorio']?>.php?modulo=sistema/geral/ver_justificativa&acao=A&tp=2&cod="+cod;          
          window.open(e, "Justificativa","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=550,height=480");
  }    

  function tomar_decisao(cod)
  {
  	if( window.confirm( "Confirma a decis�o?") )
    {
	    document.formulario.decisao.value = cod;
	    document.formulario.submit();
    } else return;
  }  
      
  function cancelar_decisao(cod)
  {
    if( window.confirm( "Confirma o CANCELAMENTO da decis�o?") )
    {
	    document.formulario.cdecisao.value = cod;
	    document.formulario.submit();
    } else return;
  }
        
</script>
</body>
</html>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script> 
