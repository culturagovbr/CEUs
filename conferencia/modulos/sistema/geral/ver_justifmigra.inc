<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:ver_justifmigra.inc
   Finalidade: permitir ver a justificativa de migra��o de uma a��o
   */
 if (! $_REQUEST['exc'])
 $sql="select pmajustificativa as justificativa from elabrev.proposta_migracao_acao where pmaid =".$_REQUEST['cod'];
 else if (! $_REQUEST['exc']==1)
  $sql="select peajustificativa as justificativa from elabrev.proposta_exclusao_acao where peaid =".$_REQUEST['cod'];
  else 
  $sql="select pepjustificativa as justificativa from elabrev.proposta_exclusao_programa where pepid =".$_REQUEST['cod'];  
    $RSu = $db->record_set($sql);
    $resu =  $db->carrega_registro($RSu,0);
    if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
?>
<html>
<head>
<title>Justificativa</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
</head>
<body bgcolor="#ffffff" leftmargin="0" rightmargin="0" topmargin="0" bottommargin="0" marginheight="0" marginwidth="0">
<form method="POST"  name="formulario">
    <center>
    <table width='100%' align='center' border="0" cellspacing="1" cellpadding="3" align="center" style="border: 1px Solid Silver; background-color:#f5f5f5;">
     <tr>
	 <td colspan="2" align="Center" bgcolor="#dedede">Justificativa</td>
	 </tr>
        <td colspan=2><?=campo_textarea('justificativa','N','N','','100%',25,'');?></td>
     </tr>
  </table>
</form> 
<script>
  function fechar_janela()
  {
    window.close();
  }
</script>
</body>
</html>
