<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:listaorgao.inc
   Finalidade: permitir o controle da lista de programas
   */
$modulo=$_REQUEST['modulo'] ;//
include "includes/cabecalho.inc";
print '<br>';
$sql= "select '<a href=simec.php?modulo=sistema/tabapoio/listaunidade&acao=A&orgcod=' || o.orgcod || '>' || o.orgcod || '</a>' as codigo, o.orgdsc as descricao, '(' || count(*) || ')' as total from orgao o inner join unidade u on o.orgcod=u.orgcod group by o.orgcod,o.orgdsc order by o.orgdsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista �rg�os Cadastrados.';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('C�digo','Descri��o','Total de Unidades');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
