<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:listaorgaosiorg.inc
   Finalidade: permitir o controle da lista de orgaos do siorg
   */


$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$sql= "select osicod as codigo, osinome as descricao, orgcod from orgaosiorg order by osinome";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de �rg�os no Cadastro Siorg';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('C�digo SIORG','Descri��o','C�digo');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
