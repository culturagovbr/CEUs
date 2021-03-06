<?php
	// Altera��o feita para que fique igual � os outros m�dulos do SIMEC.
	// Restante do c�digo foi mantido caso ocorra alguma eventualidade.
	$configuracao = array();
	array_push( $configuracao, 'elabrev' );
	include APPRAIZ .'seguranca/modulos/sistema/usuario/consusuario.inc';

	
	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * M�dulo: seguran�a
	 * Finalidade: Consultar informa��es a resppeito das contas dos usu�rios
	 * Data de cria��o:
	 * �ltima modifica��o: 29/08/2006
	 */

	/*$modulo = $_REQUEST['modulo'] ;
	$encontrou = false;
//	$problema = array();

	if ( $_REQUEST['formulario'] )
	{
		$usucpf   = $_REQUEST['usucpf'];
		$usunome  = $_REQUEST['usunome'];
		$codigo   = $_REQUEST['codigo'];
		$suscod   = $_REQUEST['suscod'];
		$perfil   = $_REQUEST['perfil'];
		$unidade  = $_REQUEST['unidade'];
//		$problema = (array) $_REQUEST['problema'];
	}
	else
	{
		$codigo = $_SESSION['sisid'];
	}

?>
<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br/>
<?php

	$db->cria_aba( $abacod_tela, $url, $parametros );
	if ( $_REQUEST['acao'] == 'A' )
	{
		$titulo_modulo = 'Alterar o Cadastro de Usu�rios do Simec';
	}
	if ( $_REQUEST['acao'] == 'C' )
	{
		$titulo_modulo = 'Consultar o Cadastro de Usu�rios do Simec';
	}
	monta_titulo( $titulo_modulo, '' );

?>
<form method="POST" name="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type=hidden name="modulo" value="<?= $modulo ?>"/>
	<input type=hidden name="act" value="0"/>
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
			<td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
			<td><?=campo_texto('usunome','','','',50,50,'','');?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Sistema:</td>
			<td>
				<?php
					$sql = "select s.sisid as codigo, s.sisdsc as descricao from seguranca.sistema s where s.sisstatus='A'";
					$db->monta_combo( "codigo", $sql, 'N', '&nbsp;', '', '', '', 250 );
				?>	
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Perfil:</td>
			<td>
			<?php
				$sql = "select p.pflcod as codigo, p.pfldsc as descricao from seguranca.perfil p where p.pflsuperuser = false and p.pflstatus='A' and p.sisid = ". $_SESSION['sisid'] ." order by pfldsc";
				$db->monta_combo( "perfil", $sql, 'S', '&nbsp;', '', '', '', 250 );
			?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Unidade Or�ament�ria:</td>
			<td>
			<?php

//				SELECT V�LIDO SE O BANCO ESTIVESSE CONSISTENTE
//				$sql = "select u2.unicod as codigo, u.unidsc || ' - ' || u2.unidsc as descricao
//					from public.unidade u
//					inner join public.unidade u2 on
//						u2.orgcod = u.unicod
//					where u2.unistatus='A' and u2.uniano = ". $_SESSION['exercicio'] ." order by u.unidsc, u2.unidsc";

				// SELECT PALEATIVO QUE PEGAR TODAS AS UNIDADES INDEPENDENTE DO EXERC�CIO
				$sql = "select u2.unicod as codigo, u.unidsc || ' - ' || u2.unidsc as descricao
					from public.unidade u
					inner join public.unidade u2 on
						u2.orgcod = u.unicod
					inner join seguranca.usuario usu on
						usu.unicod = u2.unicod
					inner join seguranca.usuario_sistema us on
						us.usucpf = usu.usucpf
					where u2.unistatus='A' and u2.unitpocod = 'U' and us.sisid = ". $_SESSION['sisid'] ."
					group by u2.unicod, u.unidsc, u2.unidsc
					order by u2.unicod, u.unidsc, u2.unidsc";
				$db->monta_combo( "unidade", $sql, 'S', '&nbsp;', '', '', '', 250 );
			?>
			</td>
		</tr>
		<!--
		<tr>
			<td align='right' class="SubTituloDireita">Problemas:</td>
			<td>
				<input id="problema_prefil" type="checkbox" name="problema[]" value="perfil" <?//= in_array( 'perfil', $problema ) ? 'checked="checked"' : "" ?>/>
				<label for="problema_prefil">Usu�rios sem perfil</label>
				
				<input id="problema_uo" type="checkbox" name="problema[]" value="uo" <?//= in_array( 'uo', $problema )  ? 'checked="checked"' : "" ?>/>
				<label for="problema_uo">Usu�rios com perfil e sem UO</label>
			</td>
		</tr>
		-->
		<?php if ( $_REQUEST['acao'] == 'A' || $_REQUEST['acao'] == 'C' ): ?>
			<tr bgcolor="#F2F2F2">
				<td align='right' class="SubTituloDireita">Status do usuario no sistema:</td>
				<td>
					<input id="status_qualquer" type="radio" name="suscod" value="" <?= $suscod == '' ? 'checked="checked"' : "" ?>/>
					<label for="status_qualquer">Qualquer</label>
					
					<input id="status_ativo" type="radio" name="suscod" value="A" <?= $suscod == 'A' ? 'checked="checked"' : "" ?>/>
					<label for="status_ativo">Ativo</label>
					
					<input id="status_pendente" type="radio" name="suscod" value="P" <?= $suscod == 'P' ? 'checked="checked"' : "" ?>/>
					<label for="status_pendente">Pendente</label>
					
					<input id="status_bloqueado" type="radio" name="suscod" value="B" <?= $suscod == 'B' ? 'checked="checked"' : "" ?>/>
					<label for="status_bloqueado">Bloqueado</label>
				</td>
			</tr>
		<?php endif; ?>
		<tr bgcolor="#C0C0C0">
			<td ></td>
			<td><input type='button' class="botao" name='consultar' value='Consultar' onclick="ProcuraUsuario()"</td>
		</tr>
	</table>
</form>
<?php

	unset( $nlinhas );
	unset( $sql );

	// insere filtros de consulta
	$texto = '';
	if ( $_REQUEST['usunome'] )
	{
		$texto .= " and u.usunome like '%" . str_to_upper( $_REQUEST['usunome'] ) . "%'";
	}
	if ( $_REQUEST['usucpf'] )
	{
		$texto.= " and u.usucpf like '%" . str_to_upper( corrige_cpf( $_REQUEST['usucpf'] ) ) . "%'";
	}
	if ( $_REQUEST['suscod']  )
	{
		$texto.= " and us.suscod = '" . $_REQUEST['suscod'] . "'";
	}
	if ( $_REQUEST['codigo'] )
	{
		$texto.= " and us.sisid = '" . $_REQUEST['codigo'] . "'";
	}
	
	$join_perfil = "";
	if ( $_REQUEST['perfil'] )
	{
		$join_perfil = sprintf(
			"inner join seguranca.perfilusuario pu on pu.usucpf = u.usucpf 
			inner join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflcod = %d",
			$_REQUEST['perfil']
		);
	}
	
	if ( $_REQUEST['unidade'] )
	{
		$texto.= " and uni.unicod = '" . (integer) $_REQUEST['unidade'] . "'" ;
	}
	
/*
	if ( is_array( $_REQUEST['problema'] ) )
	{
		if ( in_array( 'perfil', $_REQUEST['problema'] ) )
		{
			// ..
		}
		if ( in_array( 'uo', $_REQUEST['problema'] ) )
		{
			// ..
		}
	}
*/
	// seleciona qual a��o o bot�o que aparece na listagem deve acionar
	if ( $_REQUEST['acao'] == 'A' ) {
		$funcao = "alterar_usu";
	} else {
		$funcao = "consultar_usu";
	}
	
/*	$cabecalho = array( 'A��es', 'CPF', 'Nome Completo', '�rg�o', 'Unidade Or�ament�ria', 'Data de Solicita��o (ano/m�s/dia)' );

//	SELECT CORRETO SE O BANCO ESTIVESSE BEM ESTRUTURADO
//	$sql= "select distinct
//			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"$funcao(' || '\'' || u.usucpf || '\'' || ')\">' as acao,
//			u.usucpf,
//			u.usunome,
//			o.orgdsc,
//			uni.unidsc,
//			to_char(u.usudatainc,'dd/mm/YYYY') as data
//			from usuario AS u
//			inner join public.unidade uni on
//				uni.unicod = u.unicod
//			INNER JOIN usuario_sistema AS us
//				ON u.usucpf = us.usucpf, orgao o
//			where u.orgcod=o.orgcod  $texto
//			order by u.usunome";

	// SELECT PALEATIVO QUE PEGA OS USU�RIOS E O �RG�O RESPECTIVO SEM REPETIR OS DADOS
	$sql = "select
			'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\" Alterar Cadastro \" onclick=\"$funcao(' || '\'' || u.usucpf || '\'' || ')\">' as acao,
			u.usucpf,
			u.usunome,
			o.orgdsc,
			uni.unidsc,
			to_char(max(hu.htudata),'dd/mm/YYYY hh:ii') as data
			from seguranca.usuario as u ".$join_perfil." 
			inner join seguranca.usuario_sistema us on
				us.usucpf = u.usucpf
			left join seguranca.historicousuario hu on
				hu.usucpf = u.usucpf and hu.sisid = ". $_SESSION['sisid'] . "
			inner join ( select orgcod, orgdsc from public.orgao group by orgcod, orgdsc ) o on
				o.orgcod = u.orgcod
			inner join ( select unicod, unidsc from public.unidade where unitpocod = 'U' group by unicod, unidsc ) uni on
				uni.unicod = u.unicod
			where 1 = 1 " . $texto . 
			" group by u.usucpf,  u.usunome, o.orgdsc, uni.unidsc ";
//dump( htmlentities($sql), true );
	// exibe o resultado da consulta
	if ( $_REQUEST['formulario'] ) {
		$db->monta_lista( $sql, $cabecalho, 100, 20, '', '' ,'' );
	}

?>
<script language="javascript">

	function ProcuraUsuario() {
		if ( document.formulario.usunome ) {
			document.formulario.act.value = "procura";
			document.formulario.submit()
		}
	}

	function consultar_usu( usucpf ){
		window.location = "elabrev.php?modulo=sistema/usuario/cadusuario&acao=C&usucpf="+ usucpf;
		/* c�digo retirado: 29/08/2006
		document.formulario.act.value = "consultar";
		document.formulario.usucpf.value = cod;
		document.formulario.submit()
		*/
/*	}

	function alterar_usu( usucpf ){
		window.location = "elabrev.php?modulo=sistema/usuario/cadusuario&acao=A&usucpf="+ usucpf;
		/* c�digo retirado: 29/08/2006
		document.formulario.act.value = "alterar";
		document.formulario.usucpf.value = cod;
		document.formulario.submit()
		*/
	/*}

</script>*/