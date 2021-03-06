<?php

	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * M�dulo: Elabora��o e Revis�o dos Programas e A��es do Plano Pluri-Anual
	 * Finalidade: Controla as especificidades do cadastro de usu�rios do Sistema de Elabora��o e Revis�o dos Programas e A��es do Plano Pluri-Anual.
	 * Data de cria��o:
	 * �ltima modifica��o: 04/09/2006
	 */

	$pflcod = $_REQUEST['pflcod'];

?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php include APPRAIZ .'seguranca/modulos/sistema/usuario/incperfilusuario.inc'; ?>
	</td>
</tr>
<?php
	$sql = sprintf(
		"select distinct * from conferencia.tiporesponsabilidade tr inner join conferencia.tprperfil tp on tr.tprcod = tp.tprcod where tp.pflcod = %d and tr.tprsnvisivelperfil = 't'",
		$pflcod
	);
	$responsabilidades = $db->carregar( $sql );
?>
<?php if ( $responsabilidades ): ?>
	<?php foreach( $responsabilidades as $responsabilidade ): ?>
		<tr>
			<td align="right" class="subtitulodireita"><?= $responsabilidade['tprdsc'] ?>:</td>
			<td>
				<input type="hidden" name="perfil" value="<?= $responsabilidade['tprsigla'] ?>"/>
				<?php
					$sql = null;
					switch ( $responsabilidade['tprsigla'] ) {
						case 'A':
							/*
							$sql = sprintf(
								"select distinct
									pap.acacod,
									a.prgcod||'.'||pap.acacod||'.'||pap.unicod||' - '||a.acadsc as dscacao
								from conferencia.progacaoproposto as pap
									inner join conferencia.acao as a on pap.acacod = a.acacod and pap.prgid = a.prgid and pap.unicod = a.unicod
								where
									a.acasnrap = false and
									pap.usucpf =  '%s'",
								$cpf
							);
							*/
							$sql = sprintf(
								"select distinct
									pap.acacod,
									a.prgcod||'.'||pap.acacod||'.'||pap.unicod||' - '||a.acadsc as dscacao
								from conferencia.progacaoproposto as pap
									inner join conferencia.ppaacao_proposta as a on pap.acacod = a.acacod and pap.prgid = a.prgid and pap.unicod = a.unicod
								where
									a.acastatus = 'A' AND
									pap.usucpf =  '%s'",
								$cpf
							);
							break;
						case 'P':
							$sql = sprintf(
								"select
									pap.prgid,
									p.prgcod||' - '||p.prgdsc as dscprg
								from conferencia.progacaoproposto as pap
									left join conferencia.ppaprograma_proposta as p on pap.prgid = p.prgid
								where
									p.prgstatus = 'A' AND
									pap.usucpf = '%s'",
								$cpf
							);
							break;
						case 'U':
						case 'E':
						case 'N':
						default:
							break;
					}
					if ( $sql ) {
						$opcoes = $db->carregar( $sql );
					}
				?>
				<?php if ( !is_array( $opcoes ) OR $_SESSION['sisid'] == 4 ): ?>
					<select multiple size="5" name="proposto[<?= $responsabilidade['tprsigla'] ?>][]" id="proposto_<?= $responsabilidade['tprsigla'] ?>" style="width:500px;" onclick="especificar_perfil( '<?= $responsabilidade['tprsigla'] ?>' )" class="CampoEstilo">
						<option value="">Clique Aqui para Selecionar</option>
					</select>
					<?=obrigatorio();?>
					<br/>
				<?php else: ?>
					<select multiple size="5" name="proposto[<?= $responsabilidade['tprsigla'] ?>][]" id="proposto_<?= $responsabilidade['tprsigla'] ?>" style="width:500px;" onclick="" class="CampoEstilo">
						<?php
							foreach ( $opcoes as $opcao ) {
								extract( $opcao );
								print '<option value='. $prgid .'>'. $dscprg .'</option>';
							}
						?>
					</select>
					<?=obrigatorio();?>
				<?php endif; ?>
			</td>
		</tr>
	<?php endforeach; ?>
<?php endif; ?>
<script type="text/javascript">

	function selecionar_perfil(){
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

	/**
	 * Exibe op��es relacionadas ao perfil. (a��o, programa e projeto especial )
	 */
	function especificar_perfil( tipo ){
		document.getElementById( "proposto_"+ tipo ).selectedIndex = -1;
		switch ( tipo ) {
			case 'A':
				especifica_perfil = window.open( "../conferencia/geral/seleciona_aca.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'P':
				especifica_perfil = window.open( "../conferencia/geral/seleciona_prg.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'U':
				especifica_perfil = window.open( "../conferencia/geral/seleciona_unid.php?campo=proposto_"+ tipo, "especifica_perfil", "menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,width=500,height=480" );
				break;
			case 'E':
			case 'N':
			defaul:
				break;
		}
	}

	/**
	 * Recebe os itens selecionados pelo usu�rio na lista exibida pelo m�todo especificar_perfil()
	 */
	function retorna( objeto, tipo ) {
		campo = document.getElementById( "proposto_"+ tipo );
		tamanho = campo.options.length;
		if ( campo.options[0].value == '' ) {
			tamanho--;
		}
		if ( especifica_perfil.document.formulario.prgid[objeto].checked == true ){
			campo.options[tamanho] = new Option( especifica_perfil.document.formulario.prgdsc[objeto].value, especifica_perfil.document.formulario.prgid[objeto].value, false, false );
		} else {
			for( var i=0; i <= campo.length-1; i++ ) {
				if ( campo.options[i].value == especifica_perfil.document.formulario.prgid[objeto].value ) {
					campo.options[i] = null;
				}
			}
			if ( campo.options[0] ) {
			} else {
				campo.options[0] = new Option( 'Clique Aqui para Selecionar', '', false, false );
			}
		}
	}

</script>





























<?php
return;
	/**
	 * Sistema Integrado de Monitoramento do Minist�rio da Educa��o
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * M�dulo: Elabora��o e Revis�o dos Programas e A��es do Plano Pluri-Anual
	 * Finalidade: Controla as especificidades do cadastro de usu�rios do Sistema de Elabora��o e Revis�o dos Programas e A��es do Plano Pluri-Anual.
	 * Data de cria��o:
	 * �ltima modifica��o: 04/09/2006
	 */

	$pflcod = $_REQUEST['pflcod'];

?>
<tr>
	<td align="right" class="SubTituloDireita">Perfil desejado:</td>
	<td>
		<?php
			$sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where pflstatus='A' and seguranca.perfil.pflnivel > 1 and sisid=". $sisid ." order by descricao";
			$db->monta_combo( "pflcod", $sql,  'S', "Selecione o perfil desejado", 'selecionar_perfil', '' );
		?>
	</td>
</tr>
<?php if ( $pflcod ) : ?>
	<tr>
		<td align="right" class="subtitulodireita">Unidade proposta(s):</td>
		<td>
			<?php
				$sql = sprintf(
					"SELECT
						unicod as codigo,
						unicod||' - '||unidsc as descricao
					FROM unidade
					WHERE
						unitpocod = 'U' AND
						orgcod = 26000 AND
						unicod <> '26100' AND
						unistatus = 'A'
					ORDER BY unicod"
				);
				$db->monta_combo_multiplo( "unicod", $sql,  'S', "", '', '', '', 15 );
			?>
		</td>
	</tr>
<?php endif; ?>
<script type="text/javascript">

	function selecionar_perfil(){
		document.formulario.formulario.value = "";
		document.formulario.submit();
	}

</script>