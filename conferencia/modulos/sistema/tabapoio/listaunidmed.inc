<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:listaunidmed.inc
   Finalidade: permitir o controle da lista de unidades de medida
   */

$modulo=$_REQUEST['modulo'] ;//
include "includes/cabecalho.inc";
print '<br>';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$sql= "select u.unmcod as codigo, u.unmdsc as descricao from unidademedida u  order by u.unmdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de Unidades de Medida Cadastradas no Simec';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('C�digo','Descri��o');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
